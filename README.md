# osmocom-bb-gprs-no-hw

Modified version of osmocom-bb project where I try add this features:
- Communication with BTS via UDP socket using GSMTAP headers
- Add GPRS support to osmocom-bb

For these purposes I create new application named bts_proxy (src/host/bts_proxy/src/proxy), which communicate with BTS and layer23/mobile application. I also created the BTS application (src/host/bts_proxy/src/BTS) which create and send GSM messages with GSMTAP header to the proxy application via UDP socket. 

## What already work in proxy application
- receive messages from BTS and layer23
- send L1CTL messages to the layer23
  - L1CTL_RESET_CONF
  - L1CTL_SIM_CONF
  - L1CTL_PM_CONF
  - L1CTL_FBSB_CONF
  - L1CTL_CCCH_MODE_CONF
  - L1CTL_DATA_IND
  
## BTS
All messages are based on sniffed communication in based configuration for osmocom-bb (mobile phone with sim card)
BTS send these messages:
- BCCH - system type information 3
- BCCH - system type information 4
- BCCH - system type information 2
- BCCH - system type information 2ter
- PCH - Paging request


### Hints

- http://www.benkeser.com/obbphy.pdf
