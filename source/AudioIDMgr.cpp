#include "AudioIDMgr.h"

#include <bimp/BIMP_ImportStream.h>

namespace simp
{

CU_SINGLETON_DEFINITION(AudioIDMgr);

AudioIDMgr::AudioIDMgr()
{
}

void AudioIDMgr::LoadAudioIDs(const std::string& filepath)
{
	m_map2id.clear();
	AudioIDsLoader loader(filepath.c_str(), m_map2id);
	loader.Load();
}

void AudioIDMgr::GetAllAudioNames(const char* names[]) const
{
	std::map<std::string, AudioID>::const_iterator itr = m_map2id.begin();
	for (int i = 0; itr != m_map2id.end(); ++itr, ++i) {
		names[i] = itr->first.c_str();
	}
}

void AudioIDMgr::SetAudioPath(const std::string& name, const std::string& filepath)
{
	std::map<std::string, AudioID>::iterator itr = m_map2id.find(name);
	if (itr != m_map2id.end()) {
		itr->second.path = filepath;
		m_map2path.insert(std::make_pair(itr->second.id, itr->second.path));
	}
}

std::string AudioIDMgr::QueryAudioPath(int id) const
{
	std::string path;
	std::map<int, std::string>::const_iterator itr = 
		m_map2path.find(id);
	if (itr != m_map2path.end()) {
		path = itr->second;
	}
	return path;
}

/************************************************************************/
/* class AudioIDMgr::AudioIDsLoader                                     */
/************************************************************************/

AudioIDMgr::AudioIDsLoader::
AudioIDsLoader(const std::string& filepath, std::map<std::string, AudioID>& audios)
	: FileLoader(filepath)
	, m_audios(audios)
{
}

void AudioIDMgr::AudioIDsLoader::
OnLoad(bimp::ImportStream& is)
{
	int n = is.UInt16();
	for (int i = 0; i < n; ++i) 
	{
		std::string name = is.String();
		AudioID audio;
		audio.id = is.UInt32();
		m_audios.insert(std::make_pair(name, audio));
	}
}

}