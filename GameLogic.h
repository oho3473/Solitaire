#pragma once
#include<list>
#include"Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW = 5;
		const int BOARD_COLUM = 8;
		const Gdiplus::RectF mCountRect{ 885.0f,60.0f,120.0f,30.0f };

	private:
		std::unique_ptr < Gdiplus::Image> mBackground;
		std::list<Card> mDeck;

		HWND mHwnd;
		int mFlipCount;
		Card* mpSelectedCard;

	public:
		void Init(HWND hwnd);
		void Release();
		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);

	private:
		void CreateCards();
	};
}



