#ifndef __XAUDIO2_INCLUDED__
#define __XAUDIO2_INCLUDED__

#define INITGUID
#include <objbase.h>
#include <mmreg.h>

#pragma pack(push, 1)

#define XAUDIO2_MAX_BUFFER_BYTES 0x80000000
#define XAUDIO2_MAX_QUEUED_BUFFERS 64
#define XAUDIO2_MAX_BUFFERS_SYSTEM 2
#define XAUDIO2_MAX_AUDIO_CHANNELS 64
#define XAUDIO2_MIN_SAMPLE_RATE 1000
#define XAUDIO2_MAX_SAMPLE_RATE 200000
#define XAUDIO2_MAX_VOLUME_LEVEL 16777216.0f
#define XAUDIO2_MIN_FREQ_RATIO (1 / 1024.0f)
#define XAUDIO2_MAX_FREQ_RATIO 1024.0f
#define XAUDIO2_DEFAULT_FREQ_RATIO 2.0f
#define XAUDIO2_MAX_FILTER_ONEOVERQ 1.5f
#define XAUDIO2_MAX_FILTER_FREQUENCY 1.0f
#define XAUDIO2_MAX_LOOP_COUNT 254
#define XAUDIO2_MAX_INSTANCES 8

#define XAUDIO2_COMMIT_NOW 0
#define XAUDIO2_COMMIT_ALL 0
#define XAUDIO2_INVALID_OPSET (UINT32)(-1)
#define XAUDIO2_NO_LOOP_REGION 0
#define XAUDIO2_LOOP_INFINITE 255
#define XAUDIO2_DEFAULT_CHANNELS 0
#define XAUDIO2_DEFAULT_SAMPLERATE 0

#define XAUDIO2_DEBUG_ENGINE 0x0001
#define XAUDIO2_VOICE_NOPITCH 0x0002
#define XAUDIO2_VOICE_NOSRC 0x0004
#define XAUDIO2_VOICE_USEFILTER 0x0008
#define XAUDIO2_PLAY_TAILS 0x0020
#define XAUDIO2_END_OF_STREAM 0x0040
#define XAUDIO2_SEND_USEFILTER 0x0080
#define XAUDIO2_VOICE_NOSAMPLESPLAYED 0x0100
#define XAUDIO2_STOP_ENGINE_WHEN_IDLE 0x2000
#define XAUDIO2_1024_QUANTUM 0x8000
#define XAUDIO2_NO_VIRTUAL_AUDIO_CLIENT 0x10000

#define XAUDIO2_QUANTUM_NUMERATOR 1
#define XAUDIO2_QUANTUM_DENOMINATOR 100
#define XAUDIO2_QUANTUM_MS (1000.0f * XAUDIO2_QUANTUM_NUMERATOR / XAUDIO2_QUANTUM_DENOMINATOR)

#define FACILITY_XAUDIO2 0x896
#define XAUDIO2_E_INVALID_CALL 0x88960001
#define XAUDIO2_E_XMA_DECODER_ERROR 0x88960002
#define XAUDIO2_E_XAPO_CREATION_FAILED 0x88960003
#define XAUDIO2_E_DEVICE_INVALIDATED 0x88960004

#define FWD_DECLARE(x) typedef struct x x

FWD_DECLARE(IXAudio2);
FWD_DECLARE(IXAudio2Voice);
FWD_DECLARE(IXAudio2SourceVoice);
FWD_DECLARE(IXAudio2SubmixVoice);
FWD_DECLARE(IXAudio2MasteringVoice);
FWD_DECLARE(IXAudio2EngineCallback);
FWD_DECLARE(IXAudio2VoiceCallback);


typedef UINT32 XAUDIO2_PROCESSOR;
#define Processor1 0x00000001
#define Processor2 0x00000002
#define Processor3 0x00000004
#define Processor4 0x00000008
#define Processor5 0x00000010
#define Processor6 0x00000020
#define Processor7 0x00000040
#define Processor8 0x00000080
#define Processor9 0x00000100
#define Processor10 0x00000200
#define Processor11 0x00000400
#define Processor12 0x00000800
#define Processor13 0x00001000
#define Processor14 0x00002000
#define Processor15 0x00004000
#define Processor16 0x00008000
#define Processor17 0x00010000
#define Processor18 0x00020000
#define Processor19 0x00040000
#define Processor20 0x00080000
#define Processor21 0x00100000
#define Processor22 0x00200000
#define Processor23 0x00400000
#define Processor24 0x00800000
#define Processor25 0x01000000
#define Processor26 0x02000000
#define Processor27 0x04000000
#define Processor28 0x08000000
#define Processor29 0x10000000
#define Processor30 0x20000000
#define Processor31 0x40000000
#define Processor32 0x80000000
#define XAUDIO2_ANY_PROCESSOR 0xffffffff
#define XAUDIO2_DEFAULT_PROCESSOR Processor1

typedef struct XAUDIO2_VOICE_DETAILS XAUDIO2_VOICE_DETAILS;
typedef struct XAUDIO2_SEND_DESCRIPTOR XAUDIO2_SEND_DESCRIPTOR;
typedef struct XAUDIO2_VOICE_SENDS XAUDIO2_VOICE_SENDS;
typedef struct XAUDIO2_EFFECT_DESCRIPTOR AUDIO2_EFFECT_DESCRIPTOR;
typedef struct XAUDIO2_EFFECT_CHAIN XAUDIO2_EFFECT_CHAIN;
typedef struct XAUDIO2_FILTER_PARAMETERS XAUDIO2_FILTER_PARAMETERS;


typedef struct XAUDIO2_BUFFER {
	UINT32 Flags;
	UINT32 AudioBytes;
	const BYTE *pAudioData;
	UINT32 PlayBegin;
	UINT32 PlayLength;
	UINT32 LoopBegin;
	UINT32 LoopLength;
	UINT32 LoopCount;
	void *pContext;
} XAUDIO2_BUFFER;

typedef struct XAUDIO2_VOICE_STATE {
	void *pCurrentBufferContext;
	UINT32 BuffersQueued;
	UINT64 SamplesPlayed;
} XAUDIO2_VOICE_STATE;

typedef struct XAUDIO2_BUFFER_WMA XAUDIO2_BUFFER_WMA;

typedef struct XAUDIO2_PERFORMANCE_DATA {
	UINT64 AudioCyclesSinceLastQuery;
	UINT64 TotalCyclesSinceLastQuery;
	UINT32 MinimumCyclesPerQuantum;
	UINT32 MaximumCyclesPerQuantum;
	UINT32 MemoryUsageInBytes;
	UINT32 CurrentLatencyInSamples;
	UINT32 GlitchesSinceEngineStarted;
	UINT32 ActiveSourceVoiceCount;
	UINT32 TotalSourceVoiceCount;
	UINT32 ActiveSubmixVoiceCount;
	UINT32 ActiveResamplerCount;
	UINT32 ActiveMatrixMixCount;
	UINT32 ActiveXmaSourceVoices;
	UINT32 ActiveXmaStreams;
} XAUDIO2_PERFORMANCE_DATA;

typedef struct XAUDIO2_DEBUG_CONFIGURATION {
	UINT32 TraceMask;
	UINT32 BreakMask;
	BOOL LogThreadID;
	BOOL LogFileline;
	BOOL LogFunctionName;
	BOOL LogTiming;
} XAUDIO2_DEBUG_CONFIGURATION;

#define XAUDIO2_LOG_ERRORS 0x0001
#define XAUDIO2_LOG_WARNINGS 0x0002
#define XAUDIO2_LOG_INFO 0x0004
#define XAUDIO2_LOG_DETAIL 0x0008
#define XAUDIO2_LOG_API_CALLS 0x0010
#define XAUDIO2_LOG_FUNC_CALLS 0x0020
#define XAUDIO2_LOG_TIMING 0x0040
#define XAUDIO2_LOG_LOCKS 0x0080
#define XAUDIO2_LOG_MEMORY 0x0100
#define XAUDIO2_LOG_STREAMING 0x1000

typedef struct IXAudio2 {
	struct IXAudio2Vtbl *lpVtbl;
} IXAudio2;
typedef struct IXAudio2Vtbl IXAudio2Vtbl;
struct IXAudio2Vtbl {
	STDMETHOD(QueryInterface)(IXAudio2 *, REFIID riid, void **ppvInterface);
	STDMETHOD_(ULONG, AddRef)(IXAudio2 *);
	STDMETHOD_(ULONG, Release)(IXAudio2 *);
	STDMETHOD(GetDeviceCount)(IXAudio2 *, UINT32 *pCount);
	STDMETHOD(GetDeviceDetails)(IXAudio2 *, UINT32 Index, void *pDeviceDetails);
	STDMETHOD(Initialize)(IXAudio2 *, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
	STDMETHOD(RegisterForCallbacks)(IXAudio2EngineCallback *pCallback);
	STDMETHOD_(void, UnregisterForCallbacks)(IXAudio2EngineCallback *pCallback);
	STDMETHOD(CreateSourceVoice)
	(IXAudio2 *, IXAudio2SourceVoice **ppSourceVoice, const WAVEFORMATEX *pSourceFormat, UINT32 Flags, float MaxFrequencyRatio, IXAudio2VoiceCallback *pCallback, const XAUDIO2_VOICE_SENDS *pSendList, const XAUDIO2_EFFECT_CHAIN *pEffectChain);
	STDMETHOD(CreateSubmixVoice)(IXAudio2 *, IXAudio2SubmixVoice **ppSubmixVoice, UINT32 InputChannels, UINT32 InputSampleRate, UINT32 Flags, UINT32 ProcessingStage, const XAUDIO2_VOICE_SENDS *pSendList, const XAUDIO2_EFFECT_CHAIN *pEffectChain);
	STDMETHOD(CreateMasteringVoice)(IXAudio2 *, IXAudio2MasteringVoice **ppMasteringVoice, UINT32 InputChannels, UINT32 InputSampleRate, UINT32 Flags, UINT32 DeviceIndex, const XAUDIO2_EFFECT_CHAIN *pEffectChain);
	STDMETHOD(StartEngine)(IXAudio2 *);
	STDMETHOD_(void, StopEngine)(IXAudio2 *);
	STDMETHOD(CommitChanges)(IXAudio2 *, UINT32 OperationSet);
	STDMETHOD_(void, GetPerformanceData)(IXAudio2 *, XAUDIO2_PERFORMANCE_DATA *pPerfData);
	STDMETHOD_(void, SetDebugConfiguration)(IXAudio2 *, const XAUDIO2_DEBUG_CONFIGURATION *pDebugConfiguration, void *pReserved);
};


typedef struct IXAudio2Voice {
	struct IXAudio2VoiceVtbl *lpVtbl;
} IXAudio2Voice;
typedef struct IXAudio2VoiceVtbl IXAudio2VoiceVtbl;
struct IXAudio2VoiceVtbl {
#define Declare_IXAudio2Voice_Methods()                                                                                                                                                \
	STDMETHOD_(void, GetVoiceDetails)(IXAudio2Voice *, XAUDIO2_VOICE_DETAILS * pVoiceDetails);                                                                                         \
	STDMETHOD(SetOutputVoices)(IXAudio2Voice *, const XAUDIO2_VOICE_SENDS *pSendList);                                                                                                 \
	STDMETHOD(SetEffectChain)(IXAudio2Voice *, const XAUDIO2_EFFECT_CHAIN *pEffectChain);                                                                                              \
	STDMETHOD(EnableEffect)(IXAudio2Voice *, UINT32 EffectIndex, UINT32 OperationSet);                                                                                                 \
	STDMETHOD(DisableEffect)(IXAudio2Voice *, UINT32 EffectIndex, UINT32 OperationSet);                                                                                                \
	STDMETHOD_(void, GetEffectState)(IXAudio2Voice *, UINT32 EffectIndex, BOOL * pEnabled);                                                                                            \
	STDMETHOD(SetEffectParameters)(IXAudio2Voice *, UINT32 EffectIndex, const void *pParameters, UINT32 ParametersByteSize, UINT32 OperationSet);                                      \
	STDMETHOD(GetEffectParameters)(IXAudio2Voice *, UINT32 EffectIndex, void *pParameters, UINT32 ParametersByteSize);                                                                 \
	STDMETHOD(SetFilterParameters)(IXAudio2Voice *, const XAUDIO2_FILTER_PARAMETERS *pParameters, UINT32 OperationSet);                                                                \
	STDMETHOD_(void, GetFilterParameters)(IXAudio2Voice *, XAUDIO2_FILTER_PARAMETERS * pParameters);                                                                                   \
	STDMETHOD(SetOutputFilterParameters)(IXAudio2Voice *, IXAudio2Voice * pDestinationVoice, const XAUDIO2_FILTER_PARAMETERS *pParameters, UINT32 OperationSet);                       \
	STDMETHOD_(void, GetOutputFilterParameters)(IXAudio2Voice *, IXAudio2Voice * pDestinationVoice, XAUDIO2_FILTER_PARAMETERS * pParameters);                                          \
	STDMETHOD(SetVolume)(IXAudio2Voice *, float Volume, UINT32 OperationSet);                                                                                                          \
	STDMETHOD_(void, GetVolume)(IXAudio2Voice *, float *pVolume);                                                                                                                      \
	STDMETHOD(SetChannelVolumes)(IXAudio2Voice *, UINT32 Channels, const float *pVolumes, UINT32 OperationSet);                                                                        \
	STDMETHOD_(void, GetChannelVolumes)(IXAudio2Voice *, UINT32 Channels, float *pVolumes);                                                                                            \
	STDMETHOD(SetOutputMatrix)(IXAudio2Voice *, IXAudio2Voice * pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, const float *pLevelMatrix, UINT32 OperationSet); \
	STDMETHOD_(void, GetOutputMatrix)(IXAudio2Voice *, IXAudio2Voice * pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, float *pLevelMatrix);                     \
	STDMETHOD_(void, DestroyVoice)(IXAudio2Voice *)

	Declare_IXAudio2Voice_Methods();
};


typedef struct IXAudio2SourceVoice {
	struct IXAudio2SourceVoiceVtbl *lpVtbl;
} IXAudio2SourceVoice;
typedef struct IXAudio2SourceVoiceVtbl IXAudio2SourceVoiceVtbl;
struct IXAudio2SourceVoiceVtbl {
	Declare_IXAudio2Voice_Methods();
	STDMETHOD(Start)(IXAudio2SourceVoice *, UINT32 Flags, UINT32 OperationSet);
	STDMETHOD(Stop)(IXAudio2SourceVoice *, UINT32 Flags, UINT32 OperationSet);
	STDMETHOD(SubmitSourceBuffer)(IXAudio2SourceVoice *, const XAUDIO2_BUFFER *pBuffer, const XAUDIO2_BUFFER_WMA *pBufferWMA);
	STDMETHOD(FlushSourceBuffers)(IXAudio2SourceVoice *);
	STDMETHOD(Discontinuity)(IXAudio2SourceVoice *);
	STDMETHOD(ExitLoop)(IXAudio2SourceVoice *, UINT32 OperationSet);
	STDMETHOD_(void, GetState)(IXAudio2SourceVoice *, XAUDIO2_VOICE_STATE *pVoiceState, UINT32 Flags);
	STDMETHOD(SetFrequencyRatio)(IXAudio2SourceVoice *, float Ratio, UINT32 OperationSet);
	STDMETHOD_(void, GetFrequencyRatio)(IXAudio2SourceVoice *, float *pRatio);
	STDMETHOD(SetSourceSampleRate)(IXAudio2SourceVoice *, UINT32 NewSourceSampleRate);
};


typedef struct IXAudio2MasteringVoice {
	struct IXAudio2MasteringVoiceVtbl *lpVtbl;
} IXAudio2MasteringVoice;
typedef struct IXAudio2MasteringVoiceVtbl IXAudio2MasteringVoiceVtbl;
struct IXAudio2MasteringVoiceVtbl {
	Declare_IXAudio2Voice_Methods();
	STDMETHOD(GetChannelMask)(IXAudio2MasteringVoice *, DWORD *pChannelmask);
};


typedef struct IXAudio2VoiceCallback {
	struct IXAudio2VoiceCallbackVtbl *lpVtbl;
} IXAudio2VoiceCallback;
typedef struct IXAudio2VoiceCallbackVtbl IXAudio2VoiceCallbackVtbl;
struct IXAudio2VoiceCallbackVtbl {
	STDMETHOD_(void, OnVoiceProcessingPassStart)(IXAudio2VoiceCallback *, UINT32 BytesRequired);
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(IXAudio2VoiceCallback *);
	STDMETHOD_(void, OnStreamEnd)(IXAudio2VoiceCallback *);
	STDMETHOD_(void, OnBufferStart)(IXAudio2VoiceCallback *, void *pBufferContext);
	STDMETHOD_(void, OnBufferEnd)(IXAudio2VoiceCallback *, void *pBufferContext);
	STDMETHOD_(void, OnLoopEnd)(IXAudio2VoiceCallback *, void *pBufferContext);
	STDMETHOD_(void, OnVoiceError)(IXAudio2VoiceCallback *, void *pBufferContext, HRESULT Error);
};


#define IXAudio2_CreateSourceVoice(This, ppSourceVoice, pSourceFormat, Flags, MaxFrequencyRatio, pCallback, pSendList, pEffectChain) \
	((This)->lpVtbl->CreateSourceVoice(This, ppSourceVoice, pSourceFormat, Flags, MaxFrequencyRatio, pCallback, pSendList, pEffectChain))
#define IXAudio2_CreateMasteringVoice(This, ppMasteringVoice, InputChannels, InputSampleRate, Flags, DeviceId, pEffectChain) ((This)->lpVtbl->CreateMasteringVoice(This, ppMasteringVoice, InputChannels, InputSampleRate, Flags, DeviceId, pEffectChain))
#define IXAudio2_GetPerformanceData(This, pPerfData) ((This)->lpVtbl->GetPerformanceData(This, pPerfData))
#define IXAudio2_SetDebugConfiguration(This, pDebugConfiguration, pReserved) ((This)->lpVtbl->SetDebugConfiguration(This, pDebugConfiguration, pReserved))
#define IXAudio2Voice_DestroyVoice(This) ((This)->lpVtbl->DestroyVoice(This))
#define IXAudio2SourceVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2SourceVoice_Start(This, Flags, OperationSet) ((This)->lpVtbl->Start(This, Flags, OperationSet))
#define IXAudio2SourceVoice_Stop(This, Flags, OperationSet) ((This)->lpVtbl->Stop(This, Flags, OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This, pBuffer, pBufferWMA) ((This)->lpVtbl->SubmitSourceBuffer(This, pBuffer, pBufferWMA))
#define IXAudio2SourceVoice_GetState(This, pVoiceState, Flags) ((This)->lpVtbl->GetState(This, pVoiceState, Flags))

#define DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) DEFINE_GUID(CLSID_##className, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)
#define DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) DEFINE_GUID(IID_##interfaceName, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

DEFINE_CLSID(XAudio2, 5a508685, a254, 4fba, 9b, 82, 9a, 24, b0, 03, 06, af);
DEFINE_CLSID(XAudio2_Debug, db05ea35, 0329, 4d4b, a5, 3a, 6d, ea, d0, 3d, 38, 52);
DEFINE_IID(IXAudio2, 8bcf1f58, 9fe7, 4583, 8a, c6, e2, ad, c4, 65, c8, bb);

HRESULT XAudio2Create(IXAudio2 **ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor)
{
	IXAudio2 *pXAudio2;

	HRESULT hr = CoCreateInstance((Flags & XAUDIO2_DEBUG_ENGINE) ? &CLSID_XAudio2_Debug : &CLSID_XAudio2, NULL, CLSCTX_INPROC_SERVER, &IID_IXAudio2, (void **)&pXAudio2);
	if (SUCCEEDED(hr)) {
		hr = pXAudio2->lpVtbl->Initialize(pXAudio2, Flags, XAudio2Processor);

		if (SUCCEEDED(hr)) {
			*ppXAudio2 = pXAudio2;
		} else {
			pXAudio2->lpVtbl->Release(pXAudio2);
		}
	}

	return hr;
}

#pragma pack(pop)

#endif  // #ifndef __XAUDIO2_INCLUDED__
