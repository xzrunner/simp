#ifndef _SIMP_AUDIO_ID_MGR_H_
#define _SIMP_AUDIO_ID_MGR_H_

#include <cu/cu_macro.h>

#include <bimp/FileLoader.h>

#include <map>

namespace simp
{

class AudioIDMgr
{
public:
	void LoadAudioIDs(const std::string& filepath);

	int GetAudioSize() const { return m_map2id.size(); }
	void GetAllAudioNames(const char* names[]) const;

	void SetAudioPath(const std::string& name, const std::string& filepath);
	
	std::string QueryAudioPath(int id) const;

private:
	struct AudioID
	{
		std::string path;
		int         id;
	};

	class AudioIDsLoader : public bimp::FileLoader
	{
	public:
		AudioIDsLoader(const std::string& filepath, 
			std::map<std::string, AudioID>& audios);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		std::map<std::string, AudioID>& m_audios;

	}; // AudioIDsLoader

private:
	std::map<std::string, AudioID> m_map2id;
	std::map<int, std::string> m_map2path;

	CU_SINGLETON_DECLARATION(AudioIDMgr);

}; // AudioIDMgr

}

#endif // _SIMP_AUDIO_ID_MGR_H_