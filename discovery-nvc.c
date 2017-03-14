#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
//#include <stdint.h>
//#include <uuid/uuid.h>

#include "soapH.h"
#include "soapStub.h" 

int main(int argc, char *argv[])
{
    //struct soap discovery_soap; 
    struct soap *soap; 
    int result = 0; 
	struct d__ProbeType req;
	struct d__ProbeMatchesType resp;
	struct d__ScopesType sScope;
	struct SOAP_ENV__Header header;                             //soap header
	int count = 0;
//	struct SOAP_ENV__Header header;
	char guid_string[100];
    //uuid_t uuid;
    
    //soap_init(&discovery_soap); 
    //soap = &discovery_soap; 


    soap = soap_new();                                           //init soap
    if(soap==NULL)
        return -1;

    ////////////////////////////////////////////////////////////////////////////
    // discovery test
	//uuid_generate(uuid);
    //uuid_unparse(uuid, guid_string);

//    soap_omode(soap, SOAP_IO_FLUSH|SOAP_IO_STORE);
    soap_set_namespaces(soap, namespaces); 
	//超过5秒钟没有数据就退出
	soap->recv_timeout = 5;

	soap_default_SOAP_ENV__Header(soap, &header);                            //init soap header
	header.wsa__MessageID = guid_string;
	header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
	header.wsa__Action = "http://schemas.xmllocal_soap.org/ws/2005/04/discovery/Probe";
	soap->header = &header;                                                  //bind the soap header for transport                                     

 	soap_default_d__ScopesType(soap, &sScope);
	sScope.__item = "";
	//sScope.__item = "onvif://www.onvif.org";

 	soap_default_d__ProbeType(soap, &req);
	req.Scopes = &sScope;
	req.Types = "";
	//req.Types = "dn:NetworkVideoTransmitter";

	do
	{
		soap_call___dndl__Probe(soap, "soap.udp://239.255.255.250:3702/datagram", NULL, &req, &resp); 

		if (soap->error) { 
			printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
			result = soap->error; 
			break;
		} 
		else
		{
			printf("====================================================================\r\n");
				if(soap->header->wsa__MessageID)
					printf("MessageID   : %s\r\n", soap->header->wsa__MessageID);

				//if(soap->header->wsa__RelatesTo && soap->header->wsa__RelatesTo->__item)
					//printf("RelatesTo   : %s\r\n", soap->header->wsa__RelatesTo->__item);

				if(soap->header->wsa__To)
					printf("To          : %s\r\n", soap->header->wsa__To);

				if(soap->header->wsa__Action)
					printf("Action      : %s\r\n", soap->header->wsa__Action);
			printf("====================================================================\r\n");
				
			printf("soap_call___dndl__Probe __sizeProbeMatch=%d\r\n",resp.__sizeProbeMatch);
			printf("Target EP Address       : %s\r\n", resp.ProbeMatch[0].wsa__EndpointReference.Address);
			printf("Target Type             : %s\r\n", resp.ProbeMatch[0].Types);
			printf("Target Service Address  : %s\r\n", resp.ProbeMatch[0].XAddrs);
			printf("Target Metadata Version : %d\r\n", resp.ProbeMatch[0].MetadataVersion);
			printf("Target Scopes Address   : %s\r\n", resp.ProbeMatch[0].Scopes->__item);
			printf("====================================================================\r\n");
		}
	}while(0);

	soap_destroy(soap);                                                // remove deserialized class instances (C++ only)
	soap_end(soap);                                                    // clean up and remove deserialized data
    
    ////////////////////////////////////////////////////////////////////////////
//failed:
	soap_free(soap);                                                   //detach and free runtime context
	soap_done(soap);                                                   // detach context (last use and no longer in scope)

    return result; 
}
