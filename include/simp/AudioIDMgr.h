#ifndef _SIMP_AUDIO_ID_MGR_H_
#define _SIMP_AUDIO_ID_MGR_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <bimp/FileLoader.h>

namespace simp
{

class AudioIDMgr
{
public:
	void LoadAudioIDs(const CU_STR& filepath);

	int GetAudioSize() const { return m_map2id.size(); }
	void GetAllAudioNames(const char* names[]) const;

	void SetAudioPath(const CU_STR& name, const CU_STR& filepath);
	
	CU_STR QueryAudioPath(int id) const;

private:
	struct AudioID
	{
		CU_STR path;
		int             id;
	};

	class AudioIDsLoader : public bimp::FileLoader
	{
	public:
		AudioIDsLoader(const std::string& filepath,
			CU_MAP<CU_STR, AudioID>& audios);

	protected:
		virtual void OnLoad(bimp::ImportStream& is) override;

	private:
		CU_MAP<CU_STR, AudioID>& m_audios;

	}; // AudioIDsLoader

private:
	CU_MAP<CU_STR, AudioID> m_map2id;
	CU_MAP<int, CU_STR> m_map2path;

	CU_SINGLETON_DECLARATION(AudioIDMgr);

}; // AudioIDMgr

}

#endif // _SIMP_AUDIO_ID_MGR_H_