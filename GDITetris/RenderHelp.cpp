#include "RenderHelp.h"
#include <wincodec.h>
#include <iostream>

#pragma comment(lib, "windowscodecs.lib")  // WIC ���̺귯��
#pragma comment(lib, "msimg32.lib")        // AlphaBlend �Լ��� ���Ե� ���̺귯��

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace renderHelp
{
    struct WICInitializer
    {
        WICInitializer() = default;

        ~WICInitializer()
        {
            if (m_pFactory)
            {
                m_pFactory->Release();
                m_pFactory = nullptr;
            }

            DeleteAllBitmap();

            CoUninitialize();
        }

        bool Initialize()
        {
            HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if (FAILED(hr))
            {
                m_LastError = hr;
                return false;
            }

            m_LastError = CoCreateInstance(
                CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pFactory));

            if (FAILED(m_LastError))
            {
                m_pFactory = nullptr;
                return false;
            }

            return true;
        }

        bool LoadImageFromFile(__in LPCWSTR filename, __out HBITMAP& hBitmap)
        {
            hBitmap = nullptr;
            if (m_pFactory == nullptr)
            {
                m_LastError = E_FAIL;
                return false;
            }

            m_LastError = m_pFactory->CreateDecoderFromFilename(
                filename, nullptr, GENERIC_READ,
                WICDecodeMetadataCacheOnLoad, &m_pDecoder);

            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pDecoder->GetFrame(0, &m_pFrame);
            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pFactory->CreateFormatConverter(&m_pConverter);
            if (FAILED(m_LastError))
            {
                return false;
            }

            m_LastError = m_pConverter->Initialize(m_pFrame, GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone, nullptr, 0.0f,
                WICBitmapPaletteTypeCustom);

            if (FAILED(m_LastError))
            {
                return false;
            }

            UINT width = 0, height = 0;
            m_LastError = m_pFrame->GetSize(&width, &height);

            if (FAILED(m_LastError))
            {
                return false;
            }

            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width; // ��Ʈ�� �ʺ�
            bmi.bmiHeader.biHeight = -static_cast<LONG>(height); // top-down DIB
            bmi.bmiHeader.biPlanes = 1; // ��Ʈ�� ��� ��
            bmi.bmiHeader.biBitCount = 32; // 32bpp
            bmi.bmiHeader.biCompression = BI_RGB; // ���� ����

            void* pvImageBits = nullptr;
            HDC hdc = GetDC(nullptr);
            hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvImageBits, nullptr, 0);
            ReleaseDC(nullptr, hdc);

            if (FAILED(m_LastError))
            {
                return false;
            }

            // ��Ʈ�ʿ� �ȼ� ���� (32bppPBGRA�� ��ȯ)
            m_LastError = m_pConverter->CopyPixels(nullptr, width * 4, width * height * 4, (BYTE*)pvImageBits);

            if (FAILED(m_LastError))
            {
                return false;
            }

            return true;
        }

        HRESULT GetLastError() const
        {
            return m_LastError;
        }

        void Clean()
        {
            if (m_pConverter) m_pConverter->Release();
            if (m_pFrame) m_pFrame->Release();
            if (m_pDecoder) m_pDecoder->Release();
        }

        BitmapInfo* CreateBitmapInfo(HBITMAP hBitmap)
        {
            BitmapInfo* pNewBitmap = new BitmapInfo(hBitmap);
            pNewBitmap->m_pPrevLink = pLinkStart;
            pLinkStart = pNewBitmap;

            return pNewBitmap;
        }

        void DeleteAllBitmap()
        {
            BitmapInfo* pCurBitmap;
            while (pLinkStart != nullptr)
            {
                pCurBitmap = pLinkStart;
                pLinkStart = pLinkStart->m_pPrevLink;
                delete pCurBitmap;
            }
        }

        BitmapInfo* pLinkStart = nullptr;
        

        HRESULT m_LastError = S_OK;

        IWICImagingFactory* m_pFactory = nullptr;
        IWICBitmapDecoder* m_pDecoder = nullptr;
        IWICBitmapFrameDecode* m_pFrame = nullptr;
        IWICFormatConverter* m_pConverter = nullptr;

        // WICInitializer�� ������ �� ������ �����մϴ�.
        WICInitializer(const WICInitializer&) = delete;
        WICInitializer& operator=(const WICInitializer&) = delete;
    }GWICInitializer;


    SpriteSheet* CreateSpriteSheet()
    {
        return new SpriteSheet();
    }

    BitmapInfo* CreateBitmapInfo(LPCWSTR filename)
    {
        static bool bCoInit = GWICInitializer.Initialize();
        if (false == bCoInit)
        {
            return nullptr;
        }

        HBITMAP hBitmap = nullptr;
        BitmapInfo* pBitmapInfo = nullptr;
        if (GWICInitializer.LoadImageFromFile(filename, hBitmap))
        {
            pBitmapInfo = GWICInitializer.CreateBitmapInfo(hBitmap);
        }

        GWICInitializer.Clean();

        return pBitmapInfo;
    }

    SpriteSheet::SpriteSheet() : m_pBitmap(nullptr), m_pSprites(nullptr), m_spriteCount(0)
    {
    }

    SpriteSheet::~SpriteSheet()
    {
        if (m_pSprites)
        {
            delete[] m_pSprites;
            m_pSprites = nullptr;
        }
    }

    bool ParseJsonSprites(const char* jsonContent, SpriteInfo* pSprites, int& spriteCount)
    {
        if (!jsonContent || !pSprites) return false;

        const char* p = jsonContent;
        int index = 0;

        // JSON �迭 ���� '[' ã��
        while (*p && *p != '[') p++;
        if (*p != '[') return false;
        p++; // '[' ���� ���ڷ� �̵�

        // ������� ���� ���
        printf("JSON �Ľ� ����\n");

        // ��ü�� �ϳ��� �Ľ�
        while (*p && *p != ']')
        {
            // ��ü ���� '{' ã��
            while (*p && *p != '{') p++;
            if (*p != '{') break;
            p++; // '{' ���� ���ڷ� �̵�

            SpriteInfo sprite = { 0 };

            // ������� ���� ���
            printf("��ü ���� �߰�\n");

            // ��ü �� �ʵ� �Ľ�
            while (*p && *p != '}')
            {
                if (strstr(p, "\"name\"") == p)
                {
                    p = strchr(p, ':');
                    if (!p) break;
                    p++; // ':' ���� ���ڷ� �̵�

                    while (*p && (*p == ' ' || *p == '\"')) p++;

                    char tempName[32] = { 0 };
                    int nameLen = 0;
                    while (p[nameLen] && p[nameLen] != '\"') nameLen++;

                    if (nameLen > 0 && nameLen < 32)
                    {
                        strncpy_s(tempName, sizeof(tempName), p, nameLen);
                        MultiByteToWideChar(CP_UTF8, 0, tempName, -1, sprite.name, 32);

                        // ������� ���� ���
                        printf("�̸� �Ľ�: %s\n", tempName);
                    }

                    p += nameLen;
                    if (*p == '\"') p++; // �ݴ� ����ǥ �ǳʶٱ�

                    while (*p && *p != ',' && *p != '}') p++;
                    if (*p == ',') p++; // ��ǥ �ǳʶٱ�
                }
                else if (strstr(p, "\"x\"") == p)
                {
                    p = strchr(p, ':');
                    if (!p) break;
                    p++; // ':' ���� ���ڷ� �̵�

                    while (*p && (*p == ' ')) p++;

                    sprite.x = atoi(p);

                    // ������� ���� ���
                    printf("x �Ľ�: %d\n", sprite.x);

                    while (*p && *p != ',' && *p != '}') p++;
                    if (*p == ',') p++; // ��ǥ �ǳʶٱ�
                }
                else if (strstr(p, "\"y\"") == p)
                {
                    p = strchr(p, ':');
                    if (!p) break;
                    p++; // ':' ���� ���ڷ� �̵�

                    while (*p && (*p == ' ')) p++;

                    sprite.y = atoi(p);

                    // ������� ���� ���
                    printf("y �Ľ�: %d\n", sprite.y);

                    while (*p && *p != ',' && *p != '}') p++;
                    if (*p == ',') p++; // ��ǥ �ǳʶٱ�
                }
                else if (strstr(p, "\"width\"") == p)
                {
                    p = strchr(p, ':');
                    if (!p) break;
                    p++; // ':' ���� ���ڷ� �̵�

                    while (*p && (*p == ' ')) p++;

                    sprite.width = atoi(p);

                    // ������� ���� ���
                    printf("width �Ľ�: %d\n", sprite.width);

                    while (*p && *p != ',' && *p != '}') p++;
                    if (*p == ',') p++; // ��ǥ �ǳʶٱ�
                }
                else if (strstr(p, "\"height\"") == p)
                {
                    p = strchr(p, ':');
                    if (!p) break;
                    p++; // ':' ���� ���ڷ� �̵�

                    while (*p && (*p == ' ')) p++;

                    sprite.height = atoi(p);

                    // ������� ���� ���
                    printf("height �Ľ�: %d\n", sprite.height);

                    while (*p && *p != ',' && *p != '}') p++;
                    if (*p == ',') p++; // ��ǥ �ǳʶٱ�
                }
                else
                {
                    // �ٸ� ���ڴ� �ǳʶٱ�
                    p++;
                }
            }

            // ��ü �� '}' ã�� & �������� �̵�
            if (*p == '}')
            {
                p++;

                // ��ȿ�� ��������Ʈ���� Ȯ�� (�ּ��� ����/���� ũ�Ⱑ �־�� ��)
                if (sprite.width > 0 && sprite.height > 0)
                {
                    // ��������Ʈ ����
                    if (index < spriteCount)
                    {
                        pSprites[index++] = sprite;

                        // ������� ���� ���
                        printf("��������Ʈ #%d �߰� �Ϸ�\n", index - 1);
                    }
                }
            }

            // ���� ��ü ������ ��ǥ �ǳʶٱ�
            while (*p && *p != '{' && *p != '}' && *p != ']') p++;
        }

        // ������ �Ľ̵� ��������Ʈ �� ����
        printf("�� �Ľ̵� ��������Ʈ ��: %d\n", index);
        spriteCount = index;

        return (index > 0); // �ϳ� �̻� �Ľ̵Ǿ����� ����
    }

    bool SpriteSheet::LoadFromJson(LPCWSTR jsonFilePath)
    {
        FILE* pFile = nullptr;
        if (_wfopen_s(&pFile, jsonFilePath, L"rb") != 0 || pFile == nullptr)
        {
            return false;
        }

        fseek(pFile, 0, SEEK_END);
        long fileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        char* jsonContent = new char[fileSize + 1];
        fread(jsonContent, 1, fileSize, pFile);
        jsonContent[fileSize] = '\0';
        fclose(pFile);

        int maxSpriteCount = 8;
        if (m_pSprites)
        {
            delete[] m_pSprites;
            m_pSprites = nullptr;
        }

        m_pSprites = new SpriteInfo[maxSpriteCount];
        m_spriteCount = maxSpriteCount;

        bool result = ParseJsonSprites(jsonContent, m_pSprites, m_spriteCount);

        delete[] jsonContent;
        return result;
    }

    bool SpriteSheet::LoadSprite(LPCWSTR imageFilePath)
    {
        m_pBitmap = CreateBitmapInfo(imageFilePath);
        return (m_pBitmap != nullptr);
    }

    void SpriteSheet::DrawSprite(HDC hdc, int index, int x, int y, int width, int height)
    {
        if (index < 0 || index >= m_spriteCount || m_pBitmap == nullptr)
        {
            return ;
        }

        const SpriteInfo& sprite = m_pSprites[index];

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, m_pBitmap->GetBitmapHandle());

        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 255;
        bf.AlphaFormat = AC_SRC_ALPHA;

        AlphaBlend(hdc, x, y, width, height, memDC, sprite.x, sprite.y, sprite.width, sprite.height, bf);

        SelectObject(memDC, oldBitmap);
        DeleteObject(memDC);
    }

    int SpriteSheet::FindSpriteByName(const wchar_t* name) const
    {
        for (int i = 0; i < m_spriteCount; i++)
        {
            if (wcscmp(m_pSprites[i].name, name) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    const SpriteInfo* SpriteSheet::GetSpriteInfo(int index) const
    {
        if (index >= 0 && index < m_spriteCount)
        {
            return &m_pSprites[index];
        }
        return nullptr;
    }
}
