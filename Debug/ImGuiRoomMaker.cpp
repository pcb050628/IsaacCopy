#include "ImGuiRoomMaker.h"

#include "ImGui/include/imgui.h"
#include "Imgui/include/imgui_stdlib.h"

#include "LogManager.h"

#include "../World/Manager/GameClassContainer.h"

#include "../World/Data/GameDataManager.h"

void CImGuiRoomMaker::Init()
{
	d = std::make_shared<FRoomData>();
	gd = std::make_shared<CRoomGData>();

	Name.reserve(100);
	RoomFilePath.reserve(100);
	NameVec.resize(100);
}

void CImGuiRoomMaker::Update()
{
	if(ImGui::Begin("RoomInputWindow", &isOpen))
	{
		IDInput();
		SaveButton();

		if (ImGui::BeginPopup(ErrorPopupID.c_str()))
		{
			ImGui::Text("Somethings wrong...");
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(650, 350), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RoomGridWindow", &isOpen))
	{
		DrawGrid();
	}
	ImGui::End();
}

void CImGuiRoomMaker::DrawGrid()
{
	float borderSize = ImGui::GetFrameHeight();
	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 start = ImGui::GetWindowContentRegionMin();
	ImVec2 end = ImGui::GetWindowContentRegionMax();
	start.x += pos.x; start.y += pos.y - borderSize;
	end.x += pos.x; end.y += pos.y - borderSize;

	float ratioX = (end.x - start.x) / 13;
	float ratioY = (end.y - start.y) / 7;
	for (int i = 0; i < 7; i++)
	{
		ImGui::GetWindowDrawList()->AddLineH(start.x, end.x, pos.y + i * ratioY + 20, IM_COL32(0, 255, 0, 255), 1.f);
	}

	for (int i = 0; i < 13; i++)
	{
		ImGui::GetWindowDrawList()->AddLineV(pos.x + i * ratioX, start.y, end.y, IM_COL32(0, 255, 0, 255), 1.f);
	}

	for (int y = 0; y < 7; ++y)
	{
		for (int x = 0; x < 13; ++x)
		{
			int idx = y * 13 + x;
			if (!NameVec[idx].empty())
			{
				ImGui::SetCursorPos(ImVec2(start.x + ratioX * x, start.y + ratioY * y));
				ImGui::Text(NameVec[idx].c_str());
			}
		}
	}

	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
	{
		ImVec2 mouse = ImGui::GetMousePos();
		mouse.x -= pos.x; mouse.y -= pos.y + borderSize;
		LOG_DEBUG(mouse.x, ", ", mouse.y);

		LOG_DEBUG("CELL SIZE: ", ratioX, ", ", ratioY);

		int x = static_cast<int>(mouse.x / ratioX);
		int y = static_cast<int>(mouse.y / ratioY);
		LOG_DEBUG("SELECTED CELL POS: ", x, ", ", y);
		SelectedIdx = y * 13 + x;
	}
}

void CImGuiRoomMaker::IDInput()
{
	ImGui::InputText("Name", &Name);
	ImGui::InputText("FilePath", &RoomFilePath);

	if (-1 != SelectedIdx)
	{
		std::string text = "Selected: " + std::to_string(SelectedIdx);
		ImGui::Text(text.c_str());
		int x = SelectedIdx % 13;
		int y = (SelectedIdx - x) / 13;
		FVector2 coord = FVector2(static_cast<float>(x), static_cast<float>(y));
		ImGui::InputInt("ID", &InputID);
		if (ImGui::Button("Insert ID"))
		{
			std::string name = CGameClassContainer::GetInst()->GetName(InputID);
			if (name.empty())
			{
				ImGui::OpenPopup(ErrorPopupID.c_str());
				return;
			}

			NameVec[SelectedIdx] = name;
			d->InitObjs.push_back(FRoomObjectData(InputID, coord));
		}
		ImGui::Spacing();
		if (ImGui::Button("Remove ID"))
		{
			std::vector<FRoomObjectData>::iterator iter = d->InitObjs.begin();
			std::vector<FRoomObjectData>::iterator iterEnd = d->InitObjs.end();
			for (; iter != iterEnd; ++iter)
			{
				if (iter->Coord == coord)
				{
					d->InitObjs.erase(iter);
					break;
				}
			}
			NameVec[SelectedIdx] = std::string();
		}
	}

	if (!d->InitObjs.empty())
	{
		if(ImGui::BeginTable("Obj", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("ID");
			ImGui::TableSetupColumn("CoordX");
			ImGui::TableSetupColumn("CoordY");
			ImGui::TableHeadersRow();

			for (int i = 0; i < d->InitObjs.size(); ++i)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", d->InitObjs[i].ID);

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%d", static_cast<int>(d->InitObjs[i].Coord.x));
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%d", static_cast<int>(d->InitObjs[i].Coord.y));
			}
		}
		ImGui::EndTable();
	}
}

void CImGuiRoomMaker::SaveButton()
{
	if (ImGui::Button("Save"))
	{
		gd->ContainData(*d.get());
		std::wstring path = L"Room\\";
		path += std::wstring().assign(RoomFilePath.begin(), RoomFilePath.end());
		gd->WriteData(path.c_str());
	}
}
