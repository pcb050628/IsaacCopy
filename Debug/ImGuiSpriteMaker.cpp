#include "ImGuiSpriteMaker.h"

#include "ImGui/include/imgui.h"
#include "Imgui/include/imgui_stdlib.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../World/Data/GameDataManager.h"

#include "../World/Data/SpriteGData.h"
#include "../World/Data/GameObjectStructure.h"

void CImGuiSpriteMaker::Init()
{
	d = std::make_shared<FSpriteData>();
	gd = std::make_shared<CSpriteGData>();

	Name.reserve(100);
	TexturePath.reserve(100);
}

void CImGuiSpriteMaker::Update()
{
	if (ImGui::Begin("SpriteMaker", &isOpen))
	{
		InputInfo();
		LoadTexture();
		LoadButton();
		MakeButton();

		if (ImGui::BeginPopup(ErrorPopupID.c_str()))
		{
			ImGui::Text("Somethings wrong...");
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	DrawUpdate();
}

void CImGuiSpriteMaker::InputInfo()
{
	ImGui::InputText("SpriteName", &Name);
	ImGui::InputText("TexFileName", &TexturePath);
	
	ImGui::Spacing();
	//시작, 크기
	std::string Label = "StartCoordX";
	ImGui::DragFloat(Label.c_str(), &Start.x);
	Label = "StartCoordY";
	ImGui::DragFloat(Label.c_str(), &Start.y);

	Label = "SizeX";
	ImGui::DragFloat(Label.c_str(), &Size.x);
	Label = "SizeY";
	ImGui::DragFloat(Label.c_str(), &Size.y);
}

void CImGuiSpriteMaker::DrawUpdate()
{
	if (ImGui::Begin("Image", &isOpen))
	{
		DrawFrame();
		DrawImage();
	}
	ImGui::End();
}

void CImGuiSpriteMaker::DrawFrame()
{
	if (DrawingTex.expired())
		return;

	const FTextureInfo* info = DrawingTex.lock()->GetTexture(0);
	ImVec2 StartUV(Start.x / info->Width, Start.y / info->Height);
	ImVec2 EndUV((Size.x + Start.x) / info->Width, (Size.y + Start.y) / info->Height);
	ImGui::Image(info->SRV, ImVec2(300, 300), StartUV, EndUV);
	ImVec2 start = ImGui::GetItemRectMin();
	ImVec2 end = ImGui::GetItemRectMax();
	ImGui::GetWindowDrawList()->AddRect(start, end, IM_COL32(255, 0, 0, 255));
}

void CImGuiSpriteMaker::DrawImage()
{
	if (DrawingTex.expired())
		return;

	const FTextureInfo* info = DrawingTex.lock()->GetTexture(0);
	ImGui::Image(info->SRV, ImVec2(static_cast<float>(info->Width), static_cast<float>(info->Height)));
}

void CImGuiSpriteMaker::MakeButton()
{
	if (ImGui::Button("Make"))
	{
		if (!gd)
			gd = std::make_shared<CSpriteGData>();
		if (!d)
			d = std::make_shared<FSpriteData>();

		size_t loc1 = TexturePath.rfind('\\') + 1;
		size_t loc2 = TexturePath.rfind('.');
		std::string ntmp = TexturePath.substr(loc1, loc2 - loc1);
		std::string texName(ntmp.begin(), ntmp.end());

		d->Name = Name;
		d->TextureName = texName;
		d->TexturePath = TexturePath;
		d->UV.Start = Start;
		d->UV.Size = Size;
		gd->ContainData(*d.get());
		std::wstring path = L"Sprite\\";
		path += std::wstring().assign(Name.begin(), Name.end());
		gd->WriteData(path.c_str());
	}
}

void CImGuiSpriteMaker::LoadTexture()
{
	if (ImGui::Button("LoadImage"))
	{
		std::shared_ptr<CTextureManager> mgr = CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture);
		std::wstring fileName(TexturePath.begin(), TexturePath.end());
		std::wstring fullPath = CPathManager::FindPath("Texture");
		fullPath += fileName;
		size_t loc = fileName.find('\\');
		std::wstring wName = fileName.substr(loc, fileName.size() - loc);
		std::string Name(wName.begin(), wName.end());
		if (!mgr->LoadTexture(Name, fullPath.c_str()))
		{
			ImGui::OpenPopup(ErrorPopupID.c_str());
		}
		else
		{
			std::weak_ptr<CTexture> tex = mgr->FindTexture(Name);
			DrawingTex = tex;
		}
	}
}

void CImGuiSpriteMaker::SaveButton()
{
}

void CImGuiSpriteMaker::LoadButton()
{
	ImGui::Spacing();
	if (ImGui::Button("Load Sprite"))
	{
		std::shared_ptr<CGameDataManager> mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
		std::wstring path(Name.begin(), Name.end());
		if (!mgr->LoadDataFile<CSpriteGData>(Name, EGDataType::Sprite, path.c_str()))
		{
			ImGui::OpenPopup(ErrorPopupID.c_str());
			return;
		}

		gd = mgr->FindData<CSpriteGData>(Name, EGDataType::Sprite).lock();
		if (!gd)
		{
			ImGui::OpenPopup(ErrorPopupID.c_str());
			return;
		}

		d.reset();
		d = std::make_shared<FSpriteData>(gd->GetData());

		Name = d->Name;
		TexturePath = d->TexturePath;
		Start = d->UV.Start;
		Size = d->UV.Size;
	}
}
