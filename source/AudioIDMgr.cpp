#include "simp/AudioIDMgr.h"

#include <bimp/ImportStream.h>

namespace simp
{

CU_SINGLETON_DEFINITION(AudioIDMgr);

AudioIDMgr::AudioIDMgr()
{
}

void AudioIDMgr::LoadAudioIDs(const CU_STR& filepath)
{
	m_map2id.clear();
	AudioIDsLoader loader(filepath.c_str(), m_map2id);
	loader.Load();
}

void AudioIDMgr::GetAllAudioNames(const char* names[]) const
{
	auto itr = m_map2id.begin();
	for (int i = 0; itr != m_map2id.end(); ++itr, ++i) {
		names[i] = itr->first.c_str();
	}
}

void AudioIDMgr::SetAudioPath(const CU_STR& name, const CU_STR& filepath)
{
	auto itr = m_map2id.find(name);
	if (itr != m_map2id.end()) {
		itr->second.path = filepath;
		m_map2path.insert(std::make_pair(itr->second.id, itr->second.path));
	}
}

CU_STR AudioIDMgr::QueryAudioPath(int id) const
{
	CU_STR path;
	auto itr = m_map2path.find(id);
	if (itr != m_map2path.end()) {
		path = itr->second;
	}
	return path;
}

/************************************************************************/
/* class AudioIDMgr::AudioIDsLoader                                     */
/************************************************************************/

AudioIDMgr::AudioIDsLoader::
AudioIDsLoader(const std::string& filepath, CU_MAP<CU_STR, AudioID>& audios)
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
		auto name = is.String();
		AudioID audio;
		audio.id = is.UInt32();
		m_audios.insert(std::make_pair(name, audio));
	}
}

}