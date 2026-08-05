#include "ImGuiAnimationMaker.h"

#include "ImGui/include/imgui.h"
#include "Imgui/include/imgui_stdlib.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../World/Data/AnimGData.h"
#include "../World/Data/GameObjectStructure.h"

void CImGuiAnimationMaker::Init()
{
	d = std::make_shared<FAnimationData>();
	gd = std::make_shared<CAnimGData>();

	Name.reserve(100);
	AnimPath.reserve(100);
	TextureName.reserve(100);
	TexturePath.reserve(100);
}

void CImGuiAnimationMaker::Update()
{
	if (ImGui::Begin("AnimMaker", &isOpen))
	{
		InputInfo();
		FrameList();
		InputFrame();
		LoadTexture();
		AddFrameButton();
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

void CImGuiAnimationMaker::InputInfo()
{
	ImGui::InputText("AnimName", &Name);
	ImGui::InputText("AnimFileName", &AnimPath);
	ImGui::InputText("TexName", &TextureName);
	ImGui::InputText("TexFileName", &TexturePath);
}

void CImGuiAnimationMaker::InputFrame()
{
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

void CImGuiAnimationMaker::DrawUpdate()
{
	if (ImGui::Begin("Image", &isOpen))
	{
		DrawFrame();
		DrawImage();
	}
	ImGui::End();
}

void CImGuiAnimationMaker::DrawFrame()
{
	if (DrawingTex.expired())
		return;

	const FTextureInfo* info = DrawingTex.lock()->GetTexture(0);
	ImVec2 StartUV(Start.x / info->Width, Start.y / info->Height);
	ImVec2 EndUV((Size.x + Start.x) / info->Width, (Size.y + Start.y) / info->Height);
	ImGui::Image(info->SRV, ImVec2(300, 300), StartUV, EndUV);
	ImVec2 start = ImGui::GetItemRectMin();
	ImVec2 end= ImGui::GetItemRectMax();
	ImGui::GetWindowDrawList()->AddRect(start, end, IM_COL32(255, 0, 0, 255));
}

void CImGuiAnimationMaker::DrawImage()
{
	if (DrawingTex.expired())
		return;

	const FTextureInfo* info = DrawingTex.lock()->GetTexture(0);
	ImGui::Image(info->SRV, ImVec2(info->Width, info->Height));
}

void CImGuiAnimationMaker::FrameList()
{
	ImGui::Spacing(); ImGui::Spacing();
	if (!d->Frames.empty())
	{
		ImGui::BeginListBox("Frames");
		for (int n = 0; n < d->Frames.size(); n++)
		{
			const bool is_selected = (SelectedIdx == n);

			if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
			{
				SelectedIdx = n; // Update selection when clicked
			}
			std::string StartText = "Start X: " + std::to_string(d->Frames[n].Start.x) + ", Y:" + std::to_string(d->Frames[n].Start.y);
			std::string SizeText = "Sizes X: " + std::to_string(d->Frames[n].Size.x) + ", Y:" + std::to_string(d->Frames[n].Size.y);
			std::string FinalText = StartText + "\n" + SizeText;
			ImGui::Text(FinalText.c_str());
			// Set the initial focus when opening the list box (keyboard navigation)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}

void CImGuiAnimationMaker::AddFrameButton()
{
	if (ImGui::Button("AddFrame"))
	{
		FFrameData f;
		f.Start = Start;
		f.Size = Size;
		d->Frames.push_back(f);
	}
}

void CImGuiAnimationMaker::MakeButton()
{
	if (ImGui::Button("Make"))
	{
		if (!gd)
			gd = std::make_shared<CAnimGData>();
		if (!d)
			d = std::make_shared<FAnimationData>();

		if (d->Frames.empty())
		{
			ImGui::OpenPopup(ErrorPopupID.c_str());

		}
		else
		{
			d->Name = Name;
			d->TextureName = TextureName;
			d->TexturePath = TexturePath;
			gd->ContainData(*d.get());
			gd->MakeAnim();
			std::wstring path = L"Anim\\";
			path += std::wstring().assign(AnimPath.begin(), AnimPath.end());
			gd->WriteData(path.c_str());
		}
	}
}

void CImGuiAnimationMaker::LoadTexture()
{
	if (ImGui::Button("LoadImage"))
	{
		std::shared_ptr<CTextureManager> mgr = CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture);
		std::wstring fileName(TexturePath.begin(), TexturePath.end());
		std::wstring fullPath = CPathManager::FindPath("Texture");
		fullPath += fileName;
		if (!mgr->LoadTexture(d->TextureName, fullPath.c_str()))
		{
			ImGui::OpenPopup(ErrorPopupID.c_str());
		}
		else
		{
			std::weak_ptr<CTexture> tex = mgr->FindTexture(d->TextureName);
			DrawingTex = tex;
		}
	}
}

void CImGuiAnimationMaker::SaveAnimButton()
{
}

void CImGuiAnimationMaker::LoadAnimButton()
{

}
