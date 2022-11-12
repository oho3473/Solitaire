#include "Card.h"
#include<string>

using namespace Gdiplus;

namespace solitaire
{
	Card::Card(HWND hwnd,int index, Type type, int x, int y) :
		mHwnd{ hwnd },
		mIndex{index},
		mType{ type },
		mX{ x },
		mY{ y },
		mIsFront{ false }
	{
		mBack = std::make_unique<Image>(L"Data/card_back.png");

		std::wstring filename;

		switch (mType)
		{
			case Type::Wolf:
				filename = L"Data/card_creature_wolf.png";
				break;

			case Type::Dragon:
				filename = L"Data/card_creature_dragon.png";
				break;

			case Type::Bear:
				filename = L"Data/card_creature_bear.png";
				break;
		}

		mFront = std::make_unique<Image>(filename.c_str());
	}
	
	bool Card::CheckClicked(int x, int y)
	{
		Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());

		rct.Contains(x, y);

		if (rct.Contains(x,y))
		{
			Flip(!mIsFront);

			return true;
		}
	
		//TODO : CheckClick
		return false;
	}
	
	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
		Invalidate();
	}
	
	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
		}
		else
		{
			graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
		}
	}

	void Card::Invalidate()
	{
		RECT rct{ mX, mY, 
			static_cast<LONG>(mX + mFront->GetWidth()),
			static_cast<LONG>(mY + mFront->GetHeight())};
		
		InvalidateRect(mHwnd, &rct, false);
	}
}


