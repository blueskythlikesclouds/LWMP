#pragma once

typedef struct CPlayer CPlayer, *PCPlayer;
typedef struct Matrix34 Matrix34, *PMatrix34;
typedef struct Quaternion Quaternion, *PQuaternion;
typedef struct CTimerCounterList CTimerCounterList, *PCTimerCounterList;
typedef struct CPlayerVehicle CPlayerVehicle, *PCPlayerVehicle;
typedef struct StatePluginManager StatePluginManager, *PStatePluginManager;
typedef struct Vector3 Vector3, *PVector3;
typedef struct TouchedMovableShapeInfo TouchedMovableShapeInfo, *PTouchedMovableShapeInfo;
typedef struct CPostureMoveManager CPostureMoveManager, *PCPostureMoveManager;
typedef struct CParameterSpeed CParameterSpeed, *PCParameterSpeed;
typedef struct CPostureInputManager CPostureInputManager, *PCPostureInputManager;
typedef struct CStateGOC CStateGOC, *PCStateGOC;
typedef struct CBlackBoard CBlackBoard, *PCBlackBoard;
typedef struct SGroundInfo SGroundInfo, *PSGroundInfo;
typedef struct CParameterRing CParameterRing, *PCParameterRing;
typedef struct MoveArray_app__fnd__Handle_app__game__ColliShape_const
    MoveArray_app__fnd__Handle_app__game__ColliShape_const, *PMoveArray_app__fnd__Handle_app__game__ColliShape_const;
typedef struct PositionTracer PositionTracer, *PPositionTracer;
typedef struct CPhysics CPhysics, *PCPhysics;
typedef struct GOCTransform;
typedef struct CCameraController CCameraController, *PCCameraController;
typedef struct IAllocator IAllocator, *PIAllocator;
typedef struct GravityController GravityController, *PGravityController;
typedef struct CVisualBase CVisualBase, *PCVisualBase;
typedef struct CVisualLocaterManager CVisualLocaterManager, *PCVisualLocaterManager;
typedef struct GOCVisualModel GOCVisualModel, *PGOCVisualModel;
typedef struct GOCSound GOCSound, *PGOCSound;
typedef struct CPlayerVisual CPlayerVisual, *PCPlayerVisual;
typedef struct CVisualGOC CVisualGOC, *PCVisualGOC;
typedef struct CVisualLocater CVisualLocater, *PCVisualLocater;
typedef struct CharactorAnimationSingle CharactorAnimationSingle, *PCharactorAnimationSingle;
typedef struct GOCAnimationScript GOCAnimationScript, *PGOCAnimationScript;
typedef struct EffectList EffectList, *PEffectList;
typedef struct GOCReferenceHolderUnit GOCReferenceHolderUnit, *PGOCReferenceHolderUnit;
typedef struct CVisualUnit CVisualUnit, *PCVisualUnit;

struct CPlayer
{
    void* vtable;
    int field4;
    char gap8[0x4];
    int fieldC;
    void* messageManager;
    char gap10[0x318];
    void* components;
    char gap330[0xC];
    CPostureInputManager* postureInputManager;
    CPostureMoveManager* postureMoveManager;
    CPhysics* physics;
    CBlackBoard* blackBoard;
    void* pathService;
    char gap350[0x4];
    void* homingTargetService;
    char gap358[0x8];
    void* visualCinfo;
    char gap364[0xC];
    CPlayerVehicle* playerVehicle;
    char gap374[0x4];
    CCameraController* cameraController;
    char gap37C[0x14];
};

struct alignas(16) Matrix34
{
    float data[4][4];
};

struct alignas(16) Quaternion
{
    float x;
    float y;
    float z;
    float w;

    friend bool operator==(const Quaternion& lhs, const Quaternion& rhs)
    {
        return lhs.x == rhs.x
            && lhs.y == rhs.y
            && lhs.z == rhs.z
            && lhs.w == rhs.w;
    }

    friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
    {
        return !(lhs == rhs);
    }
};

struct CTimerCounterList
{
};

struct CPlayerVehicle
{
};

struct StatePluginManager
{
};

struct alignas(16) Vector3
{
    float x;
    float y;
    float z;
    char gapC[0x4];

    friend bool operator==(const Vector3& lhs, const Vector3& rhs)
    {
        return lhs.x == rhs.x
            && lhs.y == rhs.y
            && lhs.z == rhs.z;
    }

    friend bool operator!=(const Vector3& lhs, const Vector3& rhs)
    {
        return !(lhs == rhs);
    }
};

struct TouchedMovableShapeInfo
{
    char gap0[0x10];
    Matrix34 field10;
    Vector3 field50;
};

struct CPostureMoveManager
{
};

struct CParameterSpeed
{
    CPlayer* player;
    float field4;
    float field8;
    float fieldC;
    float field10;
    int field14;
    float field18;
};

struct CPostureInputManager
{
};

struct CStateGOC
{
    unsigned int field0;
    char gap4[0x2];
    unsigned short field6;
    unsigned int field8;
    void* fieldC;
    unsigned int field10;
    char gap14[0x34];
    unsigned int field48;
    char gap4C[0xC];
    CPlayer* player;
    IAllocator* field5C;
    char gap60[0x4];
    StatePluginManager* pluginManager;
    char gap68[0x8];
    CTimerCounterList* field70;
    unsigned int field74;
    unsigned int field78;
    unsigned int field7C;
    char gap80[0x30];
    MoveArray_app__fnd__Handle_app__game__ColliShape_const* fieldB0;
    unsigned int fieldB4;
    char gapB8[0x4];
};

struct CBlackBoard
{
    char gap0[0x20];
    int bodyMode;
    int playerNo;
    CParameterRing* ringParameter;
    char gap2C[0x1C];
};

struct SGroundInfo
{
    char gap0[0x10];
    Vector3 field10;
    Vector3 field20;
    int field30;
    int field34;
};

struct CParameterRing
{
    int ringCount;
};

struct MoveArray_app__fnd__Handle_app__game__ColliShape_const
{
};

struct PositionTracer
{
    Vector3 field0[256];
    PositionTracer* field1000;
    char gap1004[0xC];
    Vector3 field1010;
    float field1020;
    float field1024;
    int field1028;
    int field102C;
};

struct CPhysics
{
    char gap0[0x6];
    short field6;
    GravityController* field8;
    CPlayer* player;
    int field10;
    char gap14[0xC];
    Vector3 position;
    Quaternion rotation;
    Matrix34 matrix;
    Vector3 velocity;
    Vector3 previousVelocity;
    Vector3 previousGravityDirection;
    Vector3 fieldB0;
    Vector3 gravityForce;
    Vector3 fieldD0;
    Vector3 fieldE0;
    Vector3 fieldF0;
    unsigned char currentHistoryIndex;
    char gap101[0x3];
    int field104;
    char gap108[0x8];
    Vector3 positionHistory[256];
    Quaternion rotationHistory[256];
    Vector3 velocityHistory[256];
    float field3110[256];
    int field3510;
    char gap3514[0xC];
    Matrix34 viewMatrix;
    SGroundInfo groundInfo;
    void* field3598;
    char gap359C[0x4];
    SGroundInfo field35A0;
    void* field35D8;
    char gap35DC[0x4];
    Vector3 field35E0;
    char gap35F0[0x10];
    int field3600;
    char gap3604[0xC];
    float field3610;
    float field3614;
    float field3618;
    float field361C;
    Vector3 field3620;
    Vector3 field3630;
    Vector3 field3640;
    float field3650;
    CParameterSpeed* speedParameter;
    char gap3658[0x4];
    GravityController* gravityController;
    TouchedMovableShapeInfo field3660;
    TouchedMovableShapeInfo field36C0;
    PositionTracer* positionTracer;
    char gap3724[0xC];
    Vector3 field3730;
};

struct CCameraController
{
};

struct IAllocator
{
};

struct GravityController
{
    char gap0[0x14];
    float gravitySize;
    char gap18[0x78];
};

struct CVisualBase
{
    CPlayer* field0;
    CVisualGOC* field4;
    GOCSound* field8;
    unsigned int fieldC;
};

struct CVisualLocaterManager
{
    unsigned int field0;
    unsigned short field4;
    unsigned short field6;
    unsigned int field8;
    CVisualGOC* visualGoc;
    CVisualLocater* visualLocater;
    char gap14[0x14C];
    Matrix34 matrix;
};

struct GOCVisualModel
{
};

struct GOCSound
{
};

struct CPlayerVisual
{
    void* field0;
    char gap4[0x2];
    short field6;
    char gap8[0x4];
    CVisualBase fieldC;
    char* field1C;
    short field20;
    short field22;
    EffectList* field24;
    CVisualUnit* field28;
    GOCReferenceHolderUnit** gocReferenceHolder;
};

struct CVisualGOC
{
    unsigned int field0;
    unsigned short field4;
    unsigned short field6;
    unsigned int field8;
    void* fieldC;
    unsigned int field10;
    char gap14[0x4];
    unsigned int field18;
    unsigned int field1C;
    unsigned int field20;
    float field24;
    char gap28[0x10];
    CPlayerVisual* currentVisual;
    CPlayerVisual* humanVisual;
    CVisualLocaterManager* visualLocaterManager;
    CPlayer* player;
};

struct CVisualLocater
{
    void** vtable;
    char gap4[0xC];
    Matrix34 matrix;
};

struct CharactorAnimationSingle
{
};

struct GOCAnimationScript
{
    char gap0[0x48];
    CharactorAnimationSingle* characterAnimationSingle;
};

struct EffectList
{
};

struct GOCReferenceHolderUnit
{
    unsigned int field0;
    GOCVisualModel* visualModel;
    GOCAnimationScript* animationScript;
    unsigned int fieldC;
    char gap10[0x20];
    unsigned int field30;
    char gap34[0x4];
};

struct CVisualUnit
{
};
