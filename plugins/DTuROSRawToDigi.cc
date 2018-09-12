//-------------------------------------------------
//
//   Class: DTuROSRawToDigi
//
//   L1 DT uROS Raw-to-Digi
//
//
//
//   Author :
//   C. Heidemann - RWTH Aachen
//   J. Troconiz  - UAM
//   y el eterno retorno
//
//   Modification
//    M.C Fouz - CIEMAT. 14 June 2017
//    Interchanging Phi1 & Phi2 assigned wrongly 
//    Forcing the chamber to be in Wheel-1
//        Since is the good type for the demonstrator
//
//--------------------------------------------------

#include "DTuROSRawToDigi.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DTObjects/interface/DTReadOutMapping.h"
#include "CondFormats/DataRecord/interface/DTReadOutMappingRcd.h"

#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "EventFilter/DTRawToDigi/interface/DTROChainCoding.h"

#include "UserCode/DTDPGAnalysis/interface/BunchIDfromTDC.h"

#include <iostream>


DTuROSRawToDigi::DTuROSRawToDigi(const edm::ParameterSet& pset) {

  produces<DTDigiCollection>("DTuROSDigis");

  DTuROSInputTag_ = pset.getParameter<edm::InputTag>("DTuROS_FED_Source");

  debug_ = pset.getUntrackedParameter<bool>("debug", false);

  feds_ = pset.getUntrackedParameter<std::vector<int> >("feds", std::vector<int>());
 
  nfeds_ = feds_.size();

  Raw_token = consumes<FEDRawDataCollection>(DTuROSInputTag_);

}


DTuROSRawToDigi::~DTuROSRawToDigi(){}


void DTuROSRawToDigi::produce(edm::Event& e, const edm::EventSetup& c) {

  DTDigiCollection digis;

  if (!fillRawData(e, c, digis)) return;

  auto uROSDTDigi_product = std::make_unique<DTDigiCollection>(digis);

  e.put(std::move(uROSDTDigi_product), "DTuROSDigis");

}


bool DTuROSRawToDigi::fillRawData(edm::Event& e, const edm::EventSetup& c, DTDigiCollection& digis) {

  edm::Handle<FEDRawDataCollection> data;
  e.getByToken(Raw_token, data);

  edm::ESHandle<DTReadOutMapping> mapping;
  c.get<DTReadOutMappingRcd>().get( mapping );

  //std::cout << " New Event.............................................................................." << std::endl;
  for (int w_i = 0; w_i < nfeds_; ++w_i) {
    process(feds_[w_i], data, mapping, digis);
  }
  
  return true;
}


void DTuROSRawToDigi::process(int DTuROSFED,
                              edm::Handle<FEDRawDataCollection> data,
                              edm::ESHandle<DTReadOutMapping> mapping,
                              DTDigiCollection& digis) {


  // Container
  std::vector<long> DTuROSWordContainer;


  // Header constituents
  int BOEevTy, DTuROSId;


  // Trailer constituents
  int chkEOE, evtLgth, CRC;


  // Hit counter
  std::map<uint32_t, int> hitOrder;


  //--> Header - line 1


  FEDRawData dturosdata = data->FEDData(DTuROSFED); 
  if ( dturosdata.size() == 0 ) return;


  LineFED=dturosdata.data(); 
  long dataWord = 0;
  int lines  = 0; // counting including header
  readline(lines, dataWord);


  BOEevTy  = ( dataWord >> 60 ) & 0xF;  // positions 60 -> 63
  DTuROSId = ( dataWord >> 8 ) & 0xFFF; // positions 8 -> 19


  if ( (BOEevTy != 0x5) || (DTuROSId != DTuROSFED) ) {
    if ( debug_ ) edm::LogWarning("dturos_unpacker") << "Not a DTuROS FED " << DTuROSFED << " or header " << std::hex << dataWord;
    return;
  }


  int newCRC = 0xFFFF;
  calcCRC(dataWord, newCRC);


  int crate = DTuROSId;


  //--> Header - line 2


  readline(lines, dataWord);
  calcCRC(dataWord, newCRC);


  int nslots = ( dataWord >> 52 ) & 0xF; // positions 52 -> 55


  //--> AMC - line 3 to 2+nslots
  std::map<int, int> slot_size;
  for (int j = 0; j < nslots; ++j) {

    readline(lines, dataWord); 
    calcCRC(dataWord, newCRC);


    int slot = ( dataWord >> 16 ) & 0xF; // positions 16 -> 19


    if ( (slot < 1) || (slot > 12) ) {
      if ( debug_ ) edm::LogWarning("dturos_unpacker") << "AMCnumber " << std::dec << slot << " out of range (1-12)";
        return;
    }    


    slot_size[slot] = ( dataWord >> 32 ) & 0xFFFFFF; // positions 32 -> 55

  }

  //--> DTuROS data


  std::map<int,int>::iterator sziterator = slot_size.begin();
  std::map<int,int>::iterator szitend = slot_size.end();  
  for (; sziterator != szitend; ++sziterator) {
    for (int k=0; k<sziterator->second; ++k) {

       readline(lines, dataWord);
       calcCRC(dataWord, newCRC);


       DTuROSWordContainer.push_back(dataWord);

    }
  }  


  //--> Trailer - line 1


  readline(lines, dataWord);
  calcCRC(dataWord, newCRC);


  //--> Trailer - line 2


  readline(lines, dataWord);

  chkEOE = ( dataWord >> 60 ) & 0xF; // positions 60 -> 63


  if ( chkEOE != 0xA ) {
    if ( debug_ )  edm::LogWarning("dturos_unpacker") << "Trailer " << std::hex << dataWord << " does not start with 0xA";
    return;
  }    


  evtLgth = ( dataWord >> 32 ) & 0xFFFFFF; // positions 33 ->56
  CRC     = ( dataWord >> 16 ) & 0xFFFF;   // positions 17 ->32


  calcCRC(dataWord & 0xFFFFFFFF0000FFFF, newCRC);
  if ( newCRC != CRC ) {
    if ( debug_ ) edm::LogWarning("dturos_unpacker") 
      << "Calculated CRC " << std::hex << newCRC << " differs from CRC in trailer " << std::hex << CRC;
    return;
  }


  if ( lines != evtLgth ) {
    if ( debug_ ) edm::LogWarning("dturos_unpacker") 
      << "Number of words read != event lenght " << std::dec << lines << " " << evtLgth;
    return;
  }


  //--> analyze event

 
  std::vector<long>::iterator DTuROSiterator = DTuROSWordContainer.begin();
  std::vector<long>::iterator DTuROSitend = DTuROSWordContainer.end();


  for (; DTuROSiterator != DTuROSitend; ++DTuROSiterator) {

    dataWord  = (*DTuROSiterator); // Header AMC 1


    int slot       = ( dataWord >> 56 ) & 0xF;      // positions 56 -> 59


/**** from Carsten's code ****/

   int dataLenght = (dataWord & 0xFFFFF);         // positions 0 -> 19
   int bxCounter  = (dataWord >> 20 ) & 0xFFF;    // positions 20 -> 31
   int event      = (dataWord >> 32 ) & 0xFFFFFF; // positions 32 -> 55
//   int AMC_ID     = (dataWord >> 56 ) & 0xF;      // positions 56 -> 59
   int control    = (dataWord >> 60 ) & 0xF;      // positions 59 -> 63 
//   int wheel      = uROSWheel;


std::cout << "Header:  eventId0:" << event << "   bunchid:" << bxCounter << std::endl;

bunchIDfromTDC = bxCounter ;

/**** end of from Carsten's code ****/



    
    if ( (slot < 1) || (slot > 12) ) {
      if ( debug_ ) edm::LogWarning("dturos_unpacker") << "Slot " << std::dec << slot << " out of range (1-12) in crate " << crate;
      break;
    }


    ++DTuROSiterator;
    dataWord  = (*DTuROSiterator); // Header AMC 2


    for (int k=2; k<slot_size[slot]-1; ++k) {


      ++DTuROSiterator;
      dataWord  = (*DTuROSiterator);
      int selector  = ( dataWord >> 60 ) & 0xF; // positions 60 -> 63
      int selector2 = ( dataWord >> 28 ) & 0x1; // position  28


      //bool previousSelector=false;
      if ( selector == 4 ) { // OK word 

	;

      }
      else {

	if ( selector == 1 ) { // TDC word
        //previousSelector=true;

	  int tdcTime    = ( dataWord >> 32 ) & 0x3FFF; // positions  32 -> 45
	  int tdcChannel = ( dataWord >> 46 ) & 0x1F;   // positions  46 -> 50
	  int tdcId      = ( dataWord >> 51 ) & 0x3;    // positions  51 -> 52
	  int link       = ( dataWord >> 53 ) & 0x7F;   // positions  53 -> 59
	  int kchannel   = ( dataWord >> 46 ) & 0x3FFF; // positions  46 -> 59
//
// ---- ANDREA and MARA: Fix unpacking of digi time
//
        int offset_t    =    ( dataWord >> 37 ) & 0x1FF;         // positions   5 -> 13
        int tdc_hit_t   =    ( dataWord >> 32    & 0x1F );        // positions   0 ->  4



	  int dduId = theDDU(crate, slot, link);
	  int rosId = theROS(crate, slot, link);
	  int robId = theROB(crate, slot, link);

 
	  DTROChainCoding channelIndex(dduId, rosId, robId, tdcId, tdcChannel);
	  if (hitOrder.find(channelIndex.getCode()) == hitOrder.end()) hitOrder[channelIndex.getCode()] = 0;
	  else hitOrder[channelIndex.getCode()]++;


	  int layerId[4] = {4,2,3,1};

        // Forcing the chamber to be in an specific wheel & sector
        // if needed change also in the if (selector2==0) part of code 
	  int wheelId=0, stationId=1, sectorId=4, slId=1;  // MB1 negative
	  //int wheelId=0, stationId=1, sectorId=10, slId=1; // MB1 positive 


   	  //if (slot == 11) slId = 4-slId;
   	  if (slot == 9) slId = 4-slId;  // slot =9 ==> SL3 ==> SL Phi2

        // if needed change also in the if (selector2==0) part of code 
        int firstConnector=1;
        int cellOffset=4*(firstConnector-1); 
                          // 0 if first cable on the first FE connector (starting from the left) 
                          // 4 if first cable on secord connector
                          // ...
                          // 4x(n-1) 


	  DTWireId detId = DTWireId(wheelId, stationId, sectorId, slId, layerId[kchannel%4], (kchannel/4)+1+cellOffset);
	  int wire = detId.wire();

/// ------ ANDREA and MARA: fix the tdcTime  (1/30 of BX instead of 1/32 
	  // DTDigi digi(wire, tdcTime, hitOrder[channelIndex.getCode()]);

	  DTDigi digi(wire,  offset_t*30+tdc_hit_t-1, hitOrder[channelIndex.getCode()]);
	  digis.insertDigi(detId.layerId(),digi);
printf("Do I ever pass here? %d %d\n",offset_t*30+tdc_hit_t-1,tdcTime);

	}
        else if ( selector == 0 ) { // error word

	  if (  debug_ ) edm::LogWarning("dturos_unpacker") << "Error word [" << std::dec << k << "] : " << std::hex << dataWord 
							    << std::dec << " in slot " << slot << " in crate " << crate;

	}


	if ( selector2 == 0 ) { // TDC word

	  int tdcTime    = ( dataWord ) & 0x3FFF;       // positions   0 -> 13
	  int tdcChannel = ( dataWord >> 14 ) & 0x1F;   // positions  14 -> 18
	  int tdcId      = ( dataWord >> 19 ) & 0x3;    // positions  19 -> 20
	  int link       = ( dataWord >> 21 ) & 0x7F;   // positions  21 -> 27
	  int kchannel   = ( dataWord >> 14 ) & 0x3FFF; // positions  14 -> 27

//
// ---- ANDREA and MARA: fixing digitime
//

        int offset_t    =    ( dataWord >>  5 ) & 0x1FF;         // positions   5 -> 13
        int tdc_hit_t   =    ( dataWord         & 0x1F );        // positions   0 ->  4

	  if (tdcTime == 16383) continue;


	  int dduId = theDDU(crate, slot, link);
	  int rosId = theROS(crate, slot, link);
	  int robId = theROB(crate, slot, link);


	  DTROChainCoding channelIndex(dduId, rosId, robId, tdcId, tdcChannel);
	  if (hitOrder.find(channelIndex.getCode()) == hitOrder.end()) hitOrder[channelIndex.getCode()] = 0;
	  else hitOrder[channelIndex.getCode()]++;


	  int layerId[4] = {4,2,3,1};
          
        // Forcing the chamber to be in an specific wheel & sector
        // if needed change also in the if (selector==1) part of code 
	  int wheelId=0, stationId=1, sectorId=4, slId=1;  // MB1 negative
	  //int wheelId=0, stationId=1, sectorId=10, slId=1; // MB1 positive 

	  //if (slot == 11) slId = 4-slId;
   	  if (slot == 9) slId = 4-slId;

        // if needed change also in the if (selector1==0) part of code 
        int firstConnector=1;
        int cellOffset=4*(firstConnector-1); 
                          // 0 if first cable on the first FE connector (starting from the left) 
                          // 4 if first cable on secord connector
                          // ...
                          // 4x(n-1) 


	  DTWireId detId = DTWireId(wheelId, stationId, sectorId, slId, layerId[kchannel%4], (kchannel/4)+1+cellOffset);

	  int wire = detId.wire();

//
// ---- ANDREA and MARA: fixing digitime
//	  DTDigi digi(wire, tdcTime, hitOrder[channelIndex.getCode()]);

	  DTDigi digi(wire, offset_t*30+tdc_hit_t-1, hitOrder[channelIndex.getCode()]);
        digis.insertDigi(detId.layerId(),digi);

printf("--- changing time %d %d\n",tdcTime,offset_t*30+tdc_hit_t-1);
	}
      else if ( selector2 == 1 ) { // error word

	  if (  debug_ ) edm::LogWarning("dturos_unpacker") << "Error word [" << std::dec << k << "] : " << std::hex << dataWord 
							    << std::dec << " in slot " << slot << " in crate " << crate;

	}

     } //end if Selector !=4

    } // END LOOP for (int k=2; k<slot_size[slot]-1; ++k) 


    ++DTuROSiterator;
    dataWord  = (*DTuROSiterator); // Trailer AMC

  }  // end for-loop container content


  return;
}


int DTuROSRawToDigi::theDDU(int crate, int slot, int link) {

     if (crate == 1368) {
       if (slot < 7) return 770;
       return 771;
     }
     
     if (crate == 1370) {
       if (slot > 6) return 773;
       return 774;
     }
     
     return 772;
}


int DTuROSRawToDigi::theROS(int crate, int slot, int link) {

  if (slot%6 == 5) return link+1;

  int ros = (link/24) + 3*(slot%6) - 2;
  return ros;
}


int DTuROSRawToDigi::theROB(int crate, int slot, int link) {

  if (slot%6 == 5) return 23;
 
  int rob = link%24;
  if (rob < 15) return rob;
  if (rob == 15) return 24;
  return rob-1;
}


void DTuROSRawToDigi::calcCRC(long word, int& myC) {

  int myCRC[16], D[64], C[16];

  for ( int i = 0; i < 64; ++i ) { D[i]    = (word >> i) & 0x1; }
  for ( int i = 0; i < 16; ++i ) { C[i]    = (myC>>i)  & 0x1; }

  myCRC[0] = ( D[63] + D[62] + D[61] + D[60] + D[55] + D[54] +
               D[53] + D[52] + D[51] + D[50] + D[49] + D[48] +
               D[47] + D[46] + D[45] + D[43] + D[41] + D[40] +
               D[39] + D[38] + D[37] + D[36] + D[35] + D[34] +
               D[33] + D[32] + D[31] + D[30] + D[27] + D[26] +
               D[25] + D[24] + D[23] + D[22] + D[21] + D[20] +
               D[19] + D[18] + D[17] + D[16] + D[15] + D[13] +
               D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  +
               D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
               D[0]  + C[0]  + C[1]  + C[2]  + C[3]  + C[4]  +
               C[5]  + C[6]  + C[7]  + C[12] + C[13] + C[14] +
               C[15] )%2;

  myCRC[1] = ( D[63] + D[62] + D[61] + D[56] + D[55] + D[54] +
	       D[53] + D[52] + D[51] + D[50] + D[49] + D[48] +
	       D[47] + D[46] + D[44] + D[42] + D[41] + D[40] +
	       D[39] + D[38] + D[37] + D[36] + D[35] + D[34] +
    	       D[33] + D[32] + D[31] + D[28] + D[27] + D[26] +
    	       D[25] + D[24] + D[23] + D[22] + D[21] + D[20] +
    	       D[19] + D[18] + D[17] + D[16] + D[14] + D[13] +
    	       D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  +
	       D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
	       C[0]  + C[1]  + C[2]  + C[3]  + C[4]  + C[5]  +
	       C[6]  + C[7]  + C[8]  + C[13] + C[14] + C[15] )%2;

  myCRC[2] = ( D[61] + D[60] + D[57] + D[56] + D[46] + D[42] +
	       D[31] + D[30] + D[29] + D[28] + D[16] + D[14] +
	       D[1]  + D[0]  + C[8]  + C[9]  + C[12] + C[13] )%2;

  myCRC[3] = ( D[62] + D[61] + D[58] + D[57] + D[47] + D[43] +
	       D[32] + D[31] + D[30] + D[29] + D[17] + D[15] +
	       D[2]  + D[1]  + C[9]  + C[10] + C[13] + C[14] )%2;

  myCRC[4] = ( D[63] + D[62] + D[59] + D[58] + D[48] + D[44] +
    	       D[33] + D[32] + D[31] + D[30] + D[18] + D[16] + 
	       D[3]  + D[2]  + C[0]  + C[10] + C[11] + C[14] +
	       C[15] )%2;

  myCRC[5] = ( D[63] + D[60] + D[59] + D[49] + D[45] + D[34] +
	       D[33] + D[32] + D[31] + D[19] + D[17] + D[4]  +
    	       D[3]  + C[1]  + C[11] + C[12] + C[15] )%2;

  myCRC[6] = ( D[61] + D[60] + D[50] + D[46] + D[35] + D[34] +
	       D[33] + D[32] + D[20] + D[18] + D[5]  + D[4]  +
	       C[2]  + C[12] + C[13] )%2;

  myCRC[7] = ( D[62] + D[61] + D[51] + D[47] + D[36] + D[35] +
    	       D[34] + D[33] + D[21] + D[19] + D[6]  + D[5]  +
	       C[3]  + C[13] + C[14] )%2;

  myCRC[8] = ( D[63] + D[62] + D[52] + D[48] + D[37] + D[36] +
	       D[35] + D[34] + D[22] + D[20] + D[7]  + D[6]  +
    	       C[0]  + C[4]  + C[14] + C[15] )%2;

  myCRC[9] = ( D[63] + D[53] + D[49] + D[38] + D[37] + D[36] +
	       D[35] + D[23] + D[21] + D[8]  + D[7]  + C[1]  +
	       C[5]  + C[15] )%2;

  myCRC[10] = ( D[54] + D[50] + D[39] + D[38] + D[37] + D[36] + 
       		D[24] + D[22] + D[9]  + D[8]  + C[2]  + C[6] )%2;

  myCRC[11] = ( D[55] + D[51] + D[40] + D[39] + D[38] + D[37] +
		D[25] + D[23] + D[10] + D[9]  + C[3]  + C[7] )%2;

  myCRC[12] = ( D[56] + D[52] + D[41] + D[40] + D[39] + D[38] +
        	D[26] + D[24] + D[11] + D[10] + C[4]  + C[8] )%2;

  myCRC[13] = ( D[57] + D[53] + D[42] + D[41] + D[40] + D[39] +
	        D[27] + D[25] + D[12] + D[11] + C[5]  + C[9] )%2;

  myCRC[14] = ( D[58] + D[54] + D[43] + D[42] + D[41] + D[40] +
        	D[28] + D[26] + D[13] + D[12] + C[6]  + C[10] )%2;

  myCRC[15] = ( D[63] + D[62] + D[61] + D[60] + D[59] + D[54] +
		D[53] + D[52] + D[51] + D[50] + D[49] + D[48] + 
	        D[47] + D[46] + D[45] + D[44] + D[42] + D[40] +
        	D[39] + D[38] + D[37] + D[36] + D[35] + D[34] + 
		D[33] + D[32] + D[31] + D[30] + D[29] + D[26] +
        	D[25] + D[24] + D[23] + D[22] + D[21] + D[20] + 
        	D[19] + D[18] + D[17] + D[16] + D[15] + D[14] +
        	D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  + 
        	D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
        	D[0]  + C[0]  + C[1]  + C[2]  + C[3]  + C[4]  + 
	        C[5]  + C[6]  + C[11] + C[12] + C[13] + C[14] +
	        C[15] )%2;

  int tempC = 0x0;  
  for ( int i = 0; i < 16 ; ++i) { tempC = tempC + ( myCRC[i] << i ); }
  myC = tempC;
  return;
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DTuROSRawToDigi);
