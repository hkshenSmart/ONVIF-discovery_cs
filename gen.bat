wsdl2h.exe -sck -t WS-typemap.dat -o remotediscovery.h remotediscovery.wsdl
soapcpp2.exe -2 -L -c remotediscovery.h -IK:\OpenSource\tools\gSOAP\gsoap-2.8\gsoap\import