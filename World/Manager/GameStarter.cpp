#include "GameStarter.h"

#include <filesystem>
#include <iostream>
#include <fstream>

#include "RenderManager.h"

#include "Asset/AssetManager.h"
#include "Asset/PathManager.h"
#include "Asset/TextureManager.h"
#include "Asset/SoundManager.h"

namespace fs = std::filesystem;

bool CGameStarter::Start()
{
	if (!LoadSound())
		return false;
	if (!LoadTexture())
		return false;

	std::shared_ptr<CSoundManager> mgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	mgr->SetGroupVolume("Character", 100);
	mgr->SetGroupVolume("BGM", 35);
	mgr->SetGroupVolume("Obstacle", 35);
	mgr->SetGroupVolume("Monster", 55);

	CRenderManager::GetInst()->CreateDepthStencilState("StencilMaskWrite", false, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, true, (UINT8)255U, (UINT8)255U,
		{ D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS },
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }, 1U);
	CRenderManager::GetInst()->CreateDepthStencilState("StencilMaskApply", false, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, true, (UINT8)255U, (UINT8)255U,
		{ D3D11_STENCIL_OP_ZERO, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_EQUAL },
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		1U);

	CRenderManager::GetInst()->CreateLayer("UI", 10, ERenderSortType::None);
	CRenderManager::GetInst()->CreateLayer("Tear", 9, ERenderSortType::None);
	CRenderManager::GetInst()->CreateLayer("Pickup", 8, ERenderSortType::Y);
	CRenderManager::GetInst()->CreateLayer("Item", 7, ERenderSortType::None);
	CRenderManager::GetInst()->CreateLayer("Head", 6, ERenderSortType::Y);
	CRenderManager::GetInst()->CreateLayer("Body", 5, ERenderSortType::Y);
	CRenderManager::GetInst()->CreateLayer("Door", 4, ERenderSortType::Y);
	CRenderManager::GetInst()->CreateLayer("Obstacle", 2, ERenderSortType::Y);

	return true;
}

bool CGameStarter::LoadSound()
{
	std::shared_ptr<CSoundManager> mgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	std::wstring path = CPathManager::FindPath("Sound");
	if (!mgr || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		return false;

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			std::string entryPath = entry.path().string();
			size_t chLoc = entryPath.rfind('\\') + 1;
			std::string channel = entryPath.substr(chLoc, entryPath.size() - chLoc);
			mgr->CreateChannelGroup(channel);

			bool loop = false;
			if (channel == "BGM")
				loop = true;

			for (const fs::directory_entry& files : fs::directory_iterator(entry))
			{
				std::string np = files.path().string();

				size_t loc1 = np.rfind('\\') + 1;
				size_t loc2 = np.rfind('.');
				std::string ntmp = np.substr(loc1, loc2 - loc1);
				std::string name(ntmp.begin(), ntmp.end());
				name = channel + "_" + name;

				mgr->LoadSoundFullPath(name, channel, loop, files.path().string().c_str());
			}
		}
		else
		{
			size_t loc = path.rfind('\\') + 1;
			std::wstring tmp = path.substr(loc, path.size() - loc);
			std::string name(tmp.begin(), tmp.end());

			mgr->LoadSoundFullPath(name, "System", true, entry.path().string().c_str());
		}
	}

	return true;
}

bool CGameStarter::LoadTexture()
{
	std::shared_ptr<CTextureManager> mgr = CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture);
	std::wstring path = CPathManager::FindPath("Texture");
	if (!mgr || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		return false;

	for (const fs::directory_entry& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			for (const fs::directory_entry& files : fs::directory_iterator(entry))
			{
				std::string np = files.path().string();

				size_t loc1 = np.rfind('\\', np.rfind('\\') + 1) + 1;
				size_t loc2 = np.rfind('.');
				std::string name = np.substr(loc1, loc2 - loc1);

				mgr->LoadTexture(name, files.path().wstring().c_str());
			}
		}
		else
		{
			size_t loc = path.rfind('\\') + 1;
			size_t loc2 = path.rfind('.');
			std::wstring tmp = path.substr(loc, loc2 - loc);
			std::string name(tmp.begin(), tmp.end());

			mgr->LoadTexture(name, entry.path().wstring().c_str());
		}
	}

	return true;
}
