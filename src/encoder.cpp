#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <hybridspi/services.h>
#include <hybridspi/xml.h>
#include <hybridspi/binary.h>
#include <hybridspi/mot.h>
#include <mot/contenttypes.h>
#include <msc/datagroups.h>
#include <msc/packets.h>

#include <curl/curl.h>

#include "downloader.h"

using namespace std;
using namespace hybridspi;
using namespace hybridspi::binary;
using namespace hybridspi::xml;
using namespace mot;
using namespace msc;

inline std::ostream& operator<< (std::ostream& stream, const std::vector<unsigned char> bytes) {
    std::string text(bytes.begin(), bytes.end());
    stream << text;
    return stream;
}

int main(int argc, char* argv[])
{
    vector<unsigned char> bytes;
    for(string line; getline(cin, line);) {
        vector<unsigned char> v(line.begin(), line.end());
        bytes.insert(bytes.end(), v.begin(), v.end());
    }

    // unmarshall the SI
    //cout << "unmarshalling SI document" << endl;
    XmlMarshaller xml;
    ServiceInfo info = xml.UnmarshallServiceInfo(bytes);

    // marshall the binary
    //cout << "marshalling to binary" << endl;
    Ensemble ensemble(0xe1, 0xc1c8);
    BinaryMarshaller binary(ensemble);
    vector<unsigned char> encoded = binary.Marshall(info);

    //cout << encoded.size() << " bytes binary" << endl;    
    
    // collate the logos
    vector<Multimedia> logos;
    for(auto &service : info.Services())
    {
        for(auto &media : service.Media())
        {
            logos.push_back(media);
        }
    }

    vector<MotObject> objects;
    RandomTransportIdGenerator id;
    
    // add SI object
    MotObject si_object(id.Next(), "SI.xml", encoded, hybridspi::ContentTypes::ServiceInformation);
    si_object.AddParameter(new ScopeId(0xe1, 0xc1c8));
    objects.push_back(si_object);
    
    Downloader downloader;
    for(auto &media : logos)
    {
        vector<unsigned char> logo_data = downloader.download(media.Location());
        //cout << "got logo bytes: " << logo_data.size() << endl;
        ContentType type = media.Content().compare("image/png") == 0 ? mot::ContentTypes::Image::PNG : mot::ContentTypes::Image::JFIF; 
        
        // grab each logo image included as a local reference
        MotObject o(id.Next(), media.Location(), logo_data, type);
        o.AddParameter(new MimeType(media.Content()));
        objects.push_back(o);
    }

	SegmentEncoder segment_encoder;
	vector<Segment> segments = segment_encoder.Encode(id.Next(), objects);
    DatagroupEncoder datagroup_encoder;
    vector<Datagroup> datagroups = datagroup_encoder.Encode(segments);
    /*for(Datagroup datagroup : datagroups)
    {
        cout << datagroup.Encode();
    }*/
    PacketEncoder packet_encoder(1, 24);
    vector<Packet> packets = packet_encoder.Encode(datagroups);
	for(Packet packet : packets)
	{
	    cout << packet.Encode();
    }

  return 0;
}
