#include "ChannelMap.h"

ChannelMap::ChannelMap() :
	m_isValid(false)
{
}

ChannelMap::ChannelMap(const std::string& filename) :
	m_isValid(false)
{
	FillMap(filename);
}

ChannelMap::~ChannelMap() {}

bool ChannelMap::FillMap(const std::string& filename)
{
	std::ifstream input(filename);
	if(!input.is_open())
	{
		m_isValid = false;
		return m_isValid;
	}

	std::string junk, type, partname;
	int gchan, id, localchan;

	Channel this_chan;

	while(input>>gchan)
	{
		this_chan.type = DetType::None;
		this_chan.local_channel = -1;
		
		input>>id>>type>>partname>>localchan;
		if(type == "BARREL" && partname == "FRONTUP")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::Barrel0FrontUp; break;
				case 1: this_chan.type = DetType::Barrel1FrontUp; break;
				case 2: this_chan.type = DetType::Barrel2FrontUp; break;
				case 3: this_chan.type = DetType::Barrel3FrontUp; break;
				case 4: this_chan.type = DetType::Barrel4FrontUp; break;
				case 5: this_chan.type = DetType::Barrel5FrontUp; break;
				case 6: this_chan.type = DetType::Barrel6FrontUp; break;
				case 7: this_chan.type = DetType::Barrel7FrontUp; break;
				case 8: this_chan.type = DetType::Barrel8FrontUp; break;
				case 9: this_chan.type = DetType::Barrel9FrontUp; break;
				case 10: this_chan.type = DetType::Barrel10FrontUp; break;
				case 11: this_chan.type = DetType::Barrel11FrontUp; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "BARREL" && partname == "FRONTDOWN")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::Barrel0FrontDn; break;
				case 1: this_chan.type = DetType::Barrel1FrontDn; break;
				case 2: this_chan.type = DetType::Barrel2FrontDn; break;
				case 3: this_chan.type = DetType::Barrel3FrontDn; break;
				case 4: this_chan.type = DetType::Barrel4FrontDn; break;
				case 5: this_chan.type = DetType::Barrel5FrontDn; break;
				case 6: this_chan.type = DetType::Barrel6FrontDn; break;
				case 7: this_chan.type = DetType::Barrel7FrontDn; break;
				case 8: this_chan.type = DetType::Barrel8FrontDn; break;
				case 9: this_chan.type = DetType::Barrel9FrontDn; break;
				case 10: this_chan.type = DetType::Barrel10FrontDn; break;
				case 11: this_chan.type = DetType::Barrel11FrontDn; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "BARREL" && partname == "BACK")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::Barrel0Back; break;
				case 1: this_chan.type = DetType::Barrel1Back; break;
				case 2: this_chan.type = DetType::Barrel2Back; break;
				case 3: this_chan.type = DetType::Barrel3Back; break;
				case 4: this_chan.type = DetType::Barrel4Back; break;
				case 5: this_chan.type = DetType::Barrel5Back; break;
				case 6: this_chan.type = DetType::Barrel6Back; break;
				case 7: this_chan.type = DetType::Barrel7Back; break;
				case 8: this_chan.type = DetType::Barrel8Back; break;
				case 9: this_chan.type = DetType::Barrel9Back; break;
				case 10: this_chan.type = DetType::Barrel10Back; break;
				case 11: this_chan.type = DetType::Barrel11Back; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "FQQQ" && partname == "RING")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::FQQQ0Ring; break;
				case 1: this_chan.type = DetType::FQQQ1Ring; break;
				case 2: this_chan.type = DetType::FQQQ2Ring; break;
				case 3: this_chan.type = DetType::FQQQ3Ring; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "FQQQ" && partname == "WEDGE")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::FQQQ0Wedge; break;
				case 1: this_chan.type = DetType::FQQQ1Wedge; break;
				case 2: this_chan.type = DetType::FQQQ2Wedge; break;
				case 3: this_chan.type = DetType::FQQQ3Wedge; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "BARCUPSTREAM" && partname == "FRONT")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::BarcUpstream0Front; break;
				case 1: this_chan.type = DetType::BarcUpstream1Front; break;
				case 2: this_chan.type = DetType::BarcUpstream2Front; break;
				case 3: this_chan.type = DetType::BarcUpstream3Front; break;
				case 4: this_chan.type = DetType::BarcUpstream4Front; break;
				case 5: this_chan.type = DetType::BarcUpstream5Front; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "BARCUPSTREAM" && partname == "BACK")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::BarcUpstream0Back; break;
				case 1: this_chan.type = DetType::BarcUpstream1Back; break;
				case 2: this_chan.type = DetType::BarcUpstream2Back; break;
				case 3: this_chan.type = DetType::BarcUpstream3Back; break;
				case 4: this_chan.type = DetType::BarcUpstream4Back; break;
				case 5: this_chan.type = DetType::BarcUpstream5Back; break;
			}
			this_chan.local_channel = localchan;
		}
		else if(type == "BARCDOWNSTREAM" && partname == "FRONT")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::BarcDownstream0Front; break;
				case 1: this_chan.type = DetType::BarcDownstream1Front; break;
				case 2: this_chan.type = DetType::BarcDownstream2Front; break;
				case 3: this_chan.type = DetType::BarcDownstream3Front; break;
				case 4: this_chan.type = DetType::BarcDownstream4Front; break;
				case 5: this_chan.type = DetType::BarcDownstream5Front; break;
			}
			this_chan.local_channel = localchan;
		}	
		else if(type == "BARCDOWNSTREAM" && partname == "BACK")
		{
			switch(id)
			{
				case 0: this_chan.type = DetType::BarcDownstream0Back; break;
				case 1: this_chan.type = DetType::BarcDownstream1Back; break;
				case 2: this_chan.type = DetType::BarcDownstream2Back; break;
				case 3: this_chan.type = DetType::BarcDownstream3Back; break;
				case 4: this_chan.type = DetType::BarcDownstream4Back; break;
				case 5: this_chan.type = DetType::BarcDownstream5Back; break;
			}
			this_chan.local_channel = localchan;
		}	
		m_cmap[gchan] = this_chan;
	}
	input.close();
	m_isValid = true;
	return m_isValid;
}
