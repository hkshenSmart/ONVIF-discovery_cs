//#include <uuid/uuid.h>
#include "soapH.h"
#include "soapStub.h"


SOAP_FMAC5 int SOAP_FMAC6 __dndl__Probe(struct soap* soap, struct d__ProbeType *d__Probe, struct d__ProbeMatchesType *d__ProbeMatches)
{
	static struct d__ProbeMatchType ProbeMatch;
	static struct d__ScopesType scopes;
    static char MessageID[100];
    static char szXAddr[64];
    static char szEndpointReference[64];
    static struct wsa__Relationship wsa_RelatesTo;

   	unsigned char MacAddr[8] ={0x00, 0x16, 0xe8, 0x53, 0x13, 0xc7};
    int localIP = inet_addr("192.168.7.33");  //0x6207A8C0
    //localIP = niGetLocalIPAddr(g_netEthNi);
    //memcpy(MacAddr, niGetHwAddr(g_netEthNi), 6);

    //    if(d__Probe->Types && strcasecmp(d__Probe->Types,"dn:NetworkVideoTransmitter")==0)

    sprintf(szXAddr, "http://%s/onvif/device_service", inet_ntoa(*((struct in_addr *)&localIP)));
	sprintf(szEndpointReference, "urn:uuid:11229344-5566-7788-99aa-%02x%02x%02x%02x%02x%02x",
                        MacAddr[0],MacAddr[1],MacAddr[2],MacAddr[3],MacAddr[4],MacAddr[5]);

	printf("__dndl__Probe %p, %p, %d\n", d__ProbeMatches, d__ProbeMatches->ProbeMatch, d__ProbeMatches->__sizeProbeMatch);
	soap_default_d__ProbeMatchType(soap, &ProbeMatch);
	soap_default_d__ScopesType(soap, &scopes);
	
	if (soap->header)
	{
        //uuid_t uuid;
		
        //uuid_generate(uuid);
        strcpy(MessageID, "urn:uuid:");//urn:uuid:
        //uuid_unparse(uuid, MessageID+9);

		if(soap->header->wsa__MessageID)
		{
			//printf("remote wsa__MessageID : %s\n",soap->header->wsa__MessageID);
 			soap->header->wsa__RelatesTo =&wsa_RelatesTo;
			soap_default__wsa__RelatesTo(soap, soap->header->wsa__RelatesTo);
			soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;

			soap->header->wsa__MessageID = MessageID;
			soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
			soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
		}
	}

	scopes.__item =  "onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/hardware/IPC-model onvif://www.onvif.org/name/IPC-model";
	
	ProbeMatch.wsa__EndpointReference.Address = szEndpointReference;                 //"urn:uuid:464A4854-4656-5242-4530-313035394100";
	ProbeMatch.Types = "dn:NetworkVideoTransmitter";
	ProbeMatch.Scopes = &scopes;
	ProbeMatch.XAddrs = szXAddr;                                                    //"http://192.168.7.97/onvif/device_service";
	ProbeMatch.MetadataVersion = 1;

	d__ProbeMatches->__sizeProbeMatch = 1;
	d__ProbeMatches->ProbeMatch = &ProbeMatch;

    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __dnrd__Hello(struct soap* soap, struct d__HelloType *d__Hello, struct d__ResolveType *dn__HelloResponse){return SOAP_OK;}

SOAP_FMAC5 int SOAP_FMAC6 __dnrd__Bye(struct soap* soap, struct d__ByeType *d__Bye, struct d__ResolveType *dn__ByeResponse){return SOAP_OK;}


int main(int argc, char *argv[])
{
	struct soap soap;
	struct ip_mreq mcast;
	int count=1;
	
	soap_init2(&soap, SOAP_IO_UDP|SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);
	
    soap_set_namespaces(&soap, namespaces); 
	//soap_set_mode(&soap, SOAP_C_UTFSTRING);
	
	if(!soap_valid_socket(soap_bind(&soap, NULL, 3702, 10)))
	{ 
		soap_print_fault(&soap, stderr);
		exit(1);
	}
	
	mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt(soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)
	{
		printf("setsockopt error!\n");
		return 0;
	}	

	for(;;)
	{ 
		printf("Now Accepting requests...waiting\n");
		if(soap_serve(&soap))
			soap_print_fault(&soap, stderr);
	
		soap_destroy(&soap);
		soap_end(&soap);

		printf("客户端的IP地址：\n");
		printf("Accepted count:%d, connection from IP = %lu.%lu.%lu.%lu socket = %d \r\n",
				count, ((soap.ip)>>24)&0xFF, ((soap.ip)>>16)&0xFF, ((soap.ip)>>8)&0xFF, (soap.ip)&0xFF, (soap.socket));
		count++;
		printf("====================================================================\r\n");

	}
	
	soap_done(&soap);
	
	return 0;
}
