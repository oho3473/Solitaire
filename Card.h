#pragma once
#include<Windows.h>	//gdiplus 가 windows.h를 필요
#include<gdiplus.h>
#include<memory>


namespace solitaire
{
	enum class Type
	{
		Wolf,
		Dragon,
		Bear
	};

	class Card
	{
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;
		
		HWND mHwnd;
		int mIndex;
		int mX;
		int mY;
		bool mIsFront;
		Type mType;

	public:
		Card(HWND hwnd,int index, Type type, int x, int y);

		bool CheckClicked(int x, int y);
		void Flip(bool isFront);
		void Draw(Gdiplus::Graphics& graphics);
		void Invalidate();

		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}


