//-------------------------------------------------
//
/**  \class DTAB7RawToDigi
 *
 *   L1 DT AB7 Raw-to-Digi
 *
 *
 *
 *   C. Heidemann - RWTH Aachen
 *   J. Troconiz  - UAM Madrid
 */
//
//--------------------------------------------------
#ifndef AB7Test_DTAB7RawToDigi_h
#define AB7Test_DTAB7RawToDigi_h

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/InputTag.h>

#include <string>

class DTReadOutMapping;


class DTAB7RawToDigi : public edm::EDProducer {

public:

  /// Constructor
  DTAB7RawToDigi(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~DTAB7RawToDigi();

  /// Produce digis out of raw data
  void produce(edm::Event& e, const edm::EventSetup& c);

  /// Generate and fill FED raw data for a full event
  bool fillRawData(edm::Event& e, 
		   const edm::EventSetup& c, 
		   DTDigiCollection& digis,
		   std::vector<L1MuDTChambPhDigi> & primitives);

private:
  
  edm::InputTag DTAB7InputTag_;

  bool debug_;

  int nfeds_;

  std::vector<int> feds_;

  unsigned char* LineFED;

  // Operations

  //process data

  void process(int DTAB7FED,
               edm::Handle<FEDRawDataCollection> data,
               edm::ESHandle<DTReadOutMapping> mapping,
               DTDigiCollection& digis,
	       std::vector<L1MuDTChambPhDigi> & primitives);

  // utilities
  inline void readline(int& lines, long& dataWord) {
    dataWord = *((long*)LineFED);
    LineFED += 8;
    ++lines;
  }

  void calcCRC(long word, int& myC);

  int theDDU(int crate, int slot, int link);

  int theROS(int crate, int slot, int link);

  int theROB(int crate, int slot, int link);

  edm::InputTag getDTAB7InputTag() { return DTAB7InputTag_; }
  
  edm::EDGetTokenT<FEDRawDataCollection> Raw_token;

};
#endif
