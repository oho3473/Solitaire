#include<string>
#include<vector>
#include<random>
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;
		mpSelectedCard = nullptr;

		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");
		CreateCards();
	}
	void GameLogic::Release()
	{
		mDeck.clear();
		mBackground.reset();
	}
	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		Gdiplus::PointF pos(895.0f, 20.0f);
		Gdiplus::Font font(L"맑은 고딕", 20);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));

		graphics.DrawString(L"클릭수:", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, mCountRect, &format, &brush);
	}
	void GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};

		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				pCard = &card;
				break;
			}
		}

		if (pCard != nullptr)
		{
			mFlipCount++;
			RECT rct{ 
				static_cast<LONG>(mCountRect.GetLeft()),
				static_cast<LONG>(mCountRect.GetTop()),
				static_cast<LONG>(mCountRect.GetRight()),
				static_cast<LONG>(mCountRect.GetBottom())
			};
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)
			{
				mpSelectedCard = pCard;
			}
			else
			{
				if (mpSelectedCard == pCard)
				{
					mpSelectedCard = nullptr;
				}
				else
				{
					if (pCard->GetType() == mpSelectedCard->GetType())
					{
						pCard->Invalidate();
						mpSelectedCard->Invalidate();

						mDeck.remove_if([&](Card& card)
							{
								return card.GetIndex() == mpSelectedCard->GetIndex();
							}
						);
						mDeck.remove_if([&](Card& card) { return card.GetIndex() == pCard->GetIndex(); });
						mpSelectedCard = nullptr;
					}
					else
					{
						UpdateWindow(mHwnd);
						Sleep(500);

						pCard->Flip(false);
						mpSelectedCard->Flip(false);

						mpSelectedCard = nullptr;
					}
				}
			}
		}
			
		
	}
	void GameLogic::CreateCards()
	{
		//40장의 카드 - Type vector
		std::vector<Type> types;
		while(types.size() < static_cast<size_t>(BOARD_ROW * BOARD_COLUM))
		{
			int mod = types.size() % 6;
			switch (mod)
			{
				case 0:
					types.push_back(Type::Bear);
					types.push_back(Type::Bear);
					break;
				case 2:
					types.push_back(Type::Wolf);
					types.push_back(Type::Wolf);
					break;
				case 4:
					types.push_back(Type::Dragon);
					types.push_back(Type::Dragon);
					break;
			}
		}

		//vector shuffle
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(types.begin(), types.end(), g);

		//vector -> cardlist (5x8)
		int posX{ 15 }, posY{ 10 };
		int index{};

		for (int x = 0; x < BOARD_COLUM; ++x)
		{
			posY = 10;
			for (int y = 0; y < BOARD_ROW; ++y)
			{
				mDeck.push_back(Card(mHwnd,index, types[index++], posX, posY));
				posY += 150;
			}
			posX += 110;
		}


	}
}