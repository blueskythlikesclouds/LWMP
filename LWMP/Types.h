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
typedef class CBlackBoard CBlackBoard, *PCBlackBoard;
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
typedef class CVisualGOC CVisualGOC, *PCVisualGOC;
typedef struct CVisualLocater CVisualLocater, *PCVisualLocater;
typedef struct CharactorAnimationSingle CharactorAnimationSingle, *PCharactorAnimationSingle;
typedef struct GOCAnimationScript GOCAnimationScript, *PGOCAnimationScript;
typedef struct EffectList EffectList, *PEffectList;
typedef struct GOCReferenceHolderUnit GOCReferenceHolderUnit, *PGOCReferenceHolderUnit;
typedef struct CVisualUnit CVisualUnit, *PCVisualUnit;
typedef struct SUpdateInfo;
typedef struct GameObjectHandleBase;
typedef class CGOComponent;
struct SetObjectInfo;
struct SetObjectData;

class CGOComponent
{
public:
    virtual void Release(bool onHeap);
    virtual void unknown() = 0;
    virtual void Update(float delta) = 0;
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
};

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

    operator csl::math::Quaternion() const
    {
        return csl::math::Quaternion{ x, y, z, w };
    }

    Quaternion& operator=(const csl::math::Quaternion& quat)
    {
        x = quat.x();
        y = quat.y();
        z = quat.z();
        w = quat.w();
        return *this;
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

	operator csl::math::Vector3() const
    {
        return csl::math::Vector3{ x, y, z };
    }

	Vector3& operator=(const csl::math::Vector3& vec)
    {
        x = vec.x();
        y = vec.y();
        z = vec.z();
        return *this;
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

class CBlackBoard : public app::fnd::ReferencedObject
{
public:
    char gap0[0x14]{};
    int bodyMode{};
    int playerNo{};
    CParameterRing* ringParameter{};
    char gap2C[0x1C]{};

	CBlackBoard() {}
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

class CVisualGOC : CGOComponent
{
public:
    virtual void destruct() = 0;
    virtual void unknown() = 0;
    virtual void Update(float delta) = 0;
    virtual void Initialize(void* data) = 0;
    virtual void Finalize() = 0;

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

struct SUpdateInfo
{
    float deltaTime;
    uint32_t frame;
};

struct GameObjectHandleBase 
{
    uint16_t objectID;
    void* object;
};

namespace xgame
{
    class Message
    {
    public:
        virtual void* Clone();
        uint32_t ID;
    };
}

struct CSetObjectListener
{
    void* vfptr;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    int field_198;
    int field_19C;
    int field_1A0;
    int field_1A4;
    int field_1A8;
    int field_1AC;
    int field_1B0;
    int field_1B4;
    int field_1B8;
    int field_1BC;
    int field_1C0;
    int field_1C4;
    int field_1C8;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    int field_1F8;
    int field_1FC;
    int field_200;
    int field_204;
    int field_208;
    int field_20C;
    int field_210;
    int field_214;
    int field_218;
    int field_21C;
    int field_220;
    int field_224;
    int field_228;
    int field_22C;
    int field_230;
    int field_234;
    int field_238;
    int field_23C;
    int field_240;
    int field_244;
    int field_248;
    int field_24C;
    int field_250;
    int field_254;
    int field_258;
    int field_25C;
    int field_260;
    int field_264;
    int field_268;
    int field_26C;
    int field_270;
    int field_274;
    int field_278;
    int field_27C;
    int field_280;
    int field_284;
    int field_288;
    int field_28C;
    int field_290;
    int field_294;
    int field_298;
    int field_29C;
    int field_2A0;
    int field_2A4;
    int field_2A8;
    int field_2AC;
    int field_2B0;
    int field_2B4;
    int field_2B8;
    int field_2BC;
    int field_2C0;
    int field_2C4;
    int field_2C8;
    int field_2CC;
    int field_2D0;
    int field_2D4;
    int field_2D8;
    int field_2DC;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int field_2EC;
    int field_2F0;
    int field_2F4;
    int field_2F8;
    int field_2FC;
    int field_300;
    int field_304;
    int field_308;
    int field_30C;
    int field_310;
    int field_314;
    int field_318;
    int field_31C;
    int setObjectManager;
    SetObjectInfo* setObjectInfo;
    int field_328;
    int field_32C;
    int field_330;
    int field_334;
    int field_338;
    int field_33C;
    int field_340;
    int field_344;
    int field_348;
    int field_34C;
    int field_350;
    int field_354;
    int field_358;
    int field_35C;
    int field_360;
    int field_364;
    int field_368;
    int field_36C;
    int field_370;
    int field_374;
    int field_378;
    int field_37C;
    int field_380;
    int field_384;
    int field_388;
    int field_38C;
};

/* 421 */
struct SetObjectInfo
{
    int field_0;
    int field_4;
    SetObjectData* objectData;
};

/* 422 */
struct SetObjectData
{
    uint32_t objectID;
    int field_4;
    Vector3 position;
    int field_14;
    uint32_t parentID;
    int field_1C;
};

struct alignas(16) MsgDamage
{
    void* vfptr;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int senderType;
    int field_2C;
    Vector3 field_30;
    int field_3C;
    Vector3 field_40;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
};