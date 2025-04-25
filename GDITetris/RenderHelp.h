#pragma once
#include "INC_Windows.h"

namespace renderHelp
{
    struct WICInitializer;

    class BitmapInfo
    {
    public:
        HBITMAP GetBitmapHandle() const { return m_hBitmap; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }


    private:
        friend struct WICInitializer;

        BitmapInfo() = default;

        BitmapInfo(HBITMAP hBitmap)
        {
            m_hBitmap = hBitmap;
            BITMAP bitmap; //로드 끝난 상태
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            m_width = bitmap.bmWidth;
            m_height = bitmap.bmHeight;
        }

        ~BitmapInfo()
        {
            if (m_hBitmap)
            {
                DeleteObject(m_hBitmap);
                m_hBitmap = nullptr;
            }
        }

    private:
        HBITMAP m_hBitmap = nullptr;
        BitmapInfo* m_pPrevLink = nullptr;

        int m_width = 0;
        int m_height = 0;

        BitmapInfo(const BitmapInfo&) = delete;
        BitmapInfo& operator=(const BitmapInfo&) = delete;
    };

    struct SpriteInfo
    {
        int x;
        int y;
        int width;
        int height;
        wchar_t name[32]; 
    };

    class SpriteSheet
    {
    public:
        SpriteSheet();
        ~SpriteSheet();

        bool LoadFromJson(LPCWSTR jsonFilePath);
        bool LoadSprite(LPCWSTR imageFilePath);

        void DrawSprite(HDC hdc, int index, int x, int y, int width, int height);

        int FindSpriteByName(const wchar_t* name) const;

        const SpriteInfo* GetSpriteInfo(int index) const;
        int GetSpriteCount() const { return m_spriteCount; }

    private:
        BitmapInfo* m_pBitmap;
        SpriteInfo* m_pSprites;
        int m_spriteCount;
    };

    SpriteSheet* CreateSpriteSheet();

    BitmapInfo* CreateBitmapInfo(LPCWSTR filename);
    
    //BitmapInfo* CreateFlippedBitmap(BitmapInfo* pBitmapInfo);
}
