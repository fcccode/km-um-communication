#include <Windows.h>
#include <string>
#include <iostream>

static const std::string	gsc_szSymLink	= "\\\\.\\Dispatch_Test";
static HANDLE				gs_hDriver		= INVALID_HANDLE_VALUE;

void ShowErrorMessage(const std::string & szError, DWORD dwErrorCode)
{
	char szErrorMessage[4096] = { 0 };

	auto dwFlags = DWORD(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK);
	FormatMessageA(dwFlags, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErrorMessage, _countof(szErrorMessage), NULL);

	printf("%s | Error: %u - Description: %s\n", szError.c_str(), dwErrorCode, szErrorMessage);
}

bool ReadRoutine()
{
	auto dwReadCount	= 0UL;
	char szBuff[255]	= { 0 };
	if (ReadFile(gs_hDriver, szBuff, sizeof(szBuff), &dwReadCount, NULL) == FALSE)
	{
		ShowErrorMessage("ReadFile fail!", GetLastError());
		return false;
	}

	printf("Read message content: %s Size: %u\n", szBuff, dwReadCount);
	return true;
}

bool WriteRoutine()
{
	printf("Enter message: ");

	auto szMessage = std::string("");
	std::cin >> szMessage;

	auto dwMessageSize = static_cast<DWORD>(szMessage.size());

	auto dwWriteCount = 0UL;
	if (WriteFile(gs_hDriver, szMessage.c_str(), dwMessageSize + 1, &dwWriteCount, NULL) == FALSE)
	{
		ShowErrorMessage("WriteFile fail!", GetLastError());
		return false;
	}

	printf("Message sent! Size: %u Content size: %u\n", dwWriteCount, dwMessageSize);
	return true;
}

int main()
{
	printf("Communication CLI started! Target device: %s\n", gsc_szSymLink.c_str());

	gs_hDriver = CreateFileA(gsc_szSymLink.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (!gs_hDriver || gs_hDriver == INVALID_HANDLE_VALUE)
	{
		ShowErrorMessage("CreateFileA fail!", GetLastError());
		return 1;
	}
	printf("Handle succesfully created: %p\n", gs_hDriver);

	printf("--------------------------------------\n");

	char pInput = '0';
	while (pInput != 'x')
	{
		printf("Please select:\n1 --> Read message\n2 --> Write message\nx --> Exit\n");
		std::cin >> pInput;

		switch (pInput)
		{
			case '1':
			{
				ReadRoutine();
			} break;

			case '2':
			{
				WriteRoutine();
			} break;

			case 'x':
				return 0;

			default:
				continue;
		}
	}


    return 0;
}

