#pragma once
#include "../World/Data/GameObjectStructure.h"
#include "../World/Data/RoomGData.h"

class CImGuiRoomMaker
{

private:
	std::shared_ptr<class CRoomGData> gd;
	std::shared_ptr<struct FRoomData> d;

	std::string Name;
	std::string RoomFilePath;

	int SelectedIdx = -1;
	int InputID = 0;

	std::vector<std::string> NameVec;

	std::weak_ptr<class CTexture> DrawingTex;

	std::string ErrorPopupID = "error";
	bool isOpen = true;

public:
	void Init();
	void Update();

private:
	//화면에 방 띄우고 격자 그리기
	//클릭하면 격자 좌표 찾아서 선택하기
	//선택한 상태에서 아이디를 입력하기 -> 추가하기 버튼을 눌러서 추가
	//선택한 상태에서 제거하기를 눌러서 제거

	void DrawGrid();
	void IDInput();
	void SaveButton();
};

