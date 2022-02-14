UInt32 jump = 0xE9; // jmp instruction op code
UInt32 jumpLen = 5; // 0xE9 (1 byte) plus the relative address (4 bytes)

/**************************************************
 **	A function to override some bits with a jump **
 **	so that we can inject our own code           **
***************************************************/
bool detour(BYTE* src, BYTE* dst, UInt32 len) {
	if (len < jumpLen) {
		return false;
	}
	DWORD curProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtect);
	UInt32 relAddr = dst - src - jumpLen;
	memset(src, 0x90, len);
	*src = jump;
	*(UInt32*)(src + 1) = relAddr;
	VirtualProtect(src, len, curProtect, &curProtect);
	return true;
}

/*************************************************
 **	Function to override bits, but create extra **
 **	code to run those overwritten bits          **
**************************************************/
BYTE* detourWithTrampoline(BYTE* src, BYTE* dst, UInt32 len) {
	if (len < jumpLen) {
		return false;
	}
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + jumpLen, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);
	UInt32 gatewayRelAddr = src - gateway - jumpLen;
	*(gateway + len) = jump;
	*(UInt32*)(gateway + len + 1) = gatewayRelAddr;
	if (!detour(src, dst, len)) {
		return false;
	}
	return gateway;
}