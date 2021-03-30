#pragma once

//void** const GAME_DOCUMENT = (void**)ASLR(0xFEFEF4);
//void** const MESSAGE_MANAGER = (void**)ASLR(0xFD4300);
char* const GOCTransformString = (char*)ASLR(0xD60B44);
char* const GOCCollectorString = (char*)ASLR(0xDF7DF4);
char* const GOCCharacterInputString = (char*)ASLR(0xD64248);
char* const CStateGOCString = (char*)ASLR(0xDF77D8);
char* const CVisualGOCString = (char*)ASLR(0x00E01360);
char* const CCollisionGOCString = (char*)ASLR(0xDF7E14);
char* const CEffectGOCString = (char*)ASLR(0xE012E4);
char* const GOCColliderString = (char*)ASLR(0xD6425C);
void* const CSetObjectManagerStaticClass = (void*)ASLR(0xFEF2E8);