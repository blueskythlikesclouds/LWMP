﻿#pragma once

static FUNCTION_PTR(void*, __thiscall, changeAnimation, ASLR(0x413C20), CharactorAnimationSingle* This,
                    const char* animationName);
static FUNCTION_PTR(void*, __thiscall, setAnimationFrame, ASLR(0x414550), CharactorAnimationSingle* This,
                    float animationSpeed);
static FUNCTION_PTR(const char*, __thiscall, getCurrentAnimationName, ASLR(0x414680), CharactorAnimationSingle* This);
static FUNCTION_PTR(bool, __thiscall, isCurrentAnimation, ASLR(0x414700), CharactorAnimationSingle* This,
                    const char* animationName);
static FUNCTION_PTR(void*, __cdecl, gameObjectNew, ASLR(0x49CAD0), int byteSize);
static FUNCTION_PTR(void*, __thiscall, getGameObjectComponent, ASLR(0x49D430), void* This, char* componentName);
static FUNCTION_PTR(double, __thiscall, getAnimationFrame, ASLR(0x4B3360), GOCAnimationScript* This);
static FUNCTION_PTR(bool, __thiscall, generalWindowIsSelected, ASLR(0x507430), void* This);
static FUNCTION_PTR(void*, __thiscall, generalWindowCtor, ASLR(0x507B40), void* This, void* sParam);
static FUNCTION_PTR(void, __thiscall, generalWindowSetText, ASLR(0x5087D0), void* This, void* converseData);
static FUNCTION_PTR(void*, __thiscall, updateGroundShapeInfo, ASLR(0x850F10), CPlayer* This);
static FUNCTION_PTR(void*, __thiscall, updatePlayerInformation, ASLR(0x851F20), CPlayer* This);
static FUNCTION_PTR(void*, __thiscall, setMatrix, ASLR(0x8555E0), CPhysics* This, Matrix34& matrix);
static FUNCTION_PTR(void*, __thiscall, setPosition, ASLR(0x855650), CPhysics* This, Vector3& position);
static FUNCTION_PTR(void*, __thiscall, updateHistoryData, ASLR(0x855BA0), CPhysics* This, int a2);
static FUNCTION_PTR(void*, __thiscall, changeVisual, ASLR(0x85C5F0), CStateGOC* This, int bodyMode);
static FUNCTION_PTR(void*, __thiscall, visualGocUpdate, ASLR(0x8EFBC0), CVisualGOC* This, int a2);
static FUNCTION_PTR(void*, __thiscall, updateChangeRequest, ASLR(0x8F2E00), void* This);
static FUNCTION_PTR(void*, __thiscall, changeLocator, ASLR(0x8F5090), CVisualLocaterManager* This, const char* name,
                    float a2, int a3);
static FUNCTION_PTR(void*, __thiscall, gameDocumentAddObject, ASLR(0x90B3C0), void* This, void* object);
static FUNCTION_PTR(void*, __thiscall, getComponent, ASLR(0x914370), void* This, void* componentName);
static FUNCTION_PTR(void, __cdecl, generalWindowDisappear, ASLR(0x95E3F0));
static FUNCTION_PTR(bool, __cdecl, generalWindowIsYesButtonPressed, ASLR(0x95E430));
static FUNCTION_PTR(bool, __cdecl, generalWindowIsNoButtonPressed, ASLR(0x95E470));
static FUNCTION_PTR(bool, __cdecl, generalWindowIsMiiverseButtonPressed, ASLR(0x95E4B0));
static FUNCTION_PTR(void*, __cdecl, getSteamAvatar, ASLR(0x525250), int quality, long long steamId);
static FUNCTION_PTR(void, __cdecl, resetPosition, ASLR(0x8D85F0), CStateGOC* This, Vector3& position,
                    Quaternion& rotation);
