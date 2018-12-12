//-------------------------------------------------
//
/**  \class DTuROSRawToDigi
 *
 *   L1 DT uROS Raw-to-Digi
 *
 *
 *
 *   C. Heidemann - RWTH Aachen
 *   J. Troconiz  - UAM Madrid
 */
//
//--------------------------------------------------
#ifndef uROSTest_DTuROSRawToDigi_h
#define uROSTest_DTuROSRawToDigi_h

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/InputTag.h>

#include <string>

class DTReadOutMapping;


class DTuROSRawToDigi : public edm::EDProducer {

public:

  /// Constructor
  DTuROSRawToDigi(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~DTuROSRawToDigi();

  /// Produce digis out of raw data
  void produce(edm::Event& e, const edm::EventSetup& c);

  /// Generate and fill FED raw data for a full event
  bool fillRawData(edm::Event& e, const edm::EventSetup& c, DTDigiCollection& digis);

private:
  
  edm::InputTag DTuROSInputTag_;

  bool debug_;

  int nfeds_;

  std::vector<int> feds_;

  unsigned char* LineFED;

  // Operations

  //process data

  void process(int DTuROSFED,
               edm::Handle<FEDRawDataCollection> data,
               edm::ESHandle<DTReadOutMapping> mapping,
               DTDigiCollection& digis);

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

  edm::InputTag getDTuROSInputTag() { return DTuROSInputTag_; }
  
  edm::EDGetTokenT<FEDRawDataCollection> Raw_token;

};
#endif
