﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2017, 2019, 2023 - TortoiseGit
// Copyright (C) 2017 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#include "stdafx.h"
#include "AnimationManager.h"

/// Object to handle the timer callback.
class CTimerEventHandler : public IUIAnimationTimerEventHandler
{
public:
	CTimerEventHandler() = default;

	/// Adds a new callback function for a specific StoryBoard
	void AddCallback(IUIAnimationStoryboard* ptr, std::function<void()> func)
	{
		callbacks[ptr] = func;
	}

	/// Removes the callback for the specified StoryBoard
	void RemoveCallback(IUIAnimationStoryboard* ptr)
	{
		auto foundIt = callbacks.find(ptr);
		if (foundIt == callbacks.end())
		{
			assert(false);
			return;
		}
		// when the callback is removed via the
		// NotificationAnimationEventHandler::OnStoryboardStatusChanged
		// handler, the variables won't have yet reached their final value:
		// because right after OnStoryboardStatusChanged, this timer event
		// handler OnPostUpdate() is called again.
		// since we remove the callback here, that final call won't reach
		// the callback. So we call the callback function one last time
		// right here, before we remove it.
		foundIt->second();
		callbacks.erase(foundIt);
	}

	/// Inherited via IUIAnimationTimerEventHandler
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
	{
		if (!ppvObject)
			return E_POINTER;

		if (riid == IID_IUnknown ||
			riid == IID_IUIAnimationTimerEventHandler)
		{
			*ppvObject = static_cast<IUIAnimationTimerEventHandler*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() override
	{
		return ++ref;
	}

	virtual ULONG STDMETHODCALLTYPE Release() override
	{
		if (--ref == 0)
		{
			delete this;
			return 0;
		}

		return ref;
	}

	virtual HRESULT STDMETHODCALLTYPE OnPreUpdate() override
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnPostUpdate() override
	{
		for (const auto& callback : callbacks)
			callback.second();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnRenderingTooSlow(UINT32 /*framesPerSecond*/) override
	{
		return S_OK;
	}

private:
	std::map<IUIAnimationStoryboard*, std::function<void()>> callbacks;
	unsigned long ref = 0;
};

/// object to handle StoryBoard events
class NotificationAnimationEventHandler : public IUIAnimationStoryboardEventHandler
{
public:
	/// Constructor
	NotificationAnimationEventHandler() = default;

	~NotificationAnimationEventHandler()
	{
		if (timerEventHandler)
			timerEventHandler->Release();
	}

	/// Sets the timer object event handler
	void SetTimerObj(CTimerEventHandler* handler)
	{
		if (timerEventHandler)
			timerEventHandler->Release();

		timerEventHandler = handler;
		timerEventHandler->AddRef();
	}

	/// Inherited via IUIAnimationStoryboardEventHandler
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
	{
		if (!ppvObject)
			return E_POINTER;

		if (riid == IID_IUnknown || riid == IID_IUIAnimationStoryboardEventHandler)
		{
			*ppvObject = static_cast<IUIAnimationStoryboardEventHandler*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() override
	{
		return ++ref;
	}

	virtual ULONG STDMETHODCALLTYPE Release() override
	{
		if (--ref == 0)
		{
			delete this;
			return 0;
		}

		return ref;
	}

	/// IUIAnimationStoryboardEventHandler Interface implementation
	HRESULT STDMETHODCALLTYPE OnStoryboardStatusChanged(IUIAnimationStoryboard* storyboard, UI_ANIMATION_STORYBOARD_STATUS newStatus, UI_ANIMATION_STORYBOARD_STATUS previousStatus) override
	{
		UNREFERENCED_PARAMETER(storyboard);
		UNREFERENCED_PARAMETER(previousStatus);
		if (newStatus == UI_ANIMATION_STORYBOARD_FINISHED || newStatus == UI_ANIMATION_STORYBOARD_CANCELLED || newStatus == UI_ANIMATION_STORYBOARD_TRUNCATED)
		{
			// since the StoryBoard is now not in use anymore, remove
			// the timer callback function for it.
			if (timerEventHandler)
				timerEventHandler->RemoveCallback(storyboard);
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnStoryboardUpdated(IUIAnimationStoryboard* /*storyboard*/) override
	{
		return S_OK;
	}

private:
	CTimerEventHandler* timerEventHandler = nullptr;
	unsigned long ref = 0;
};

IUIAnimationVariablePtr Animator::CreateAnimationVariable(double start)
{
	IUIAnimationVariablePtr pAnimVar = nullptr;
	if (pAnimMgr && SUCCEEDED(pAnimMgr->CreateAnimationVariable(start, &pAnimVar)))
		return pAnimVar;
	return nullptr;
}

INT32 Animator::GetIntegerValue(IUIAnimationVariablePtr var)
{
	INT32 val = 0;
	if (var)
		var->GetIntegerValue(&val);
	return val;
}

double Animator::GetValue(IUIAnimationVariablePtr var)
{
	double val = 0;
	if (var)
		var->GetValue(&val);
	return val;
}

IUIAnimationTransitionPtr Animator::CreateAccelerateDecelerateTransition(UI_ANIMATION_SECONDS duration, double finalValue, double accelerationRatio, double decelerationRatio)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateAccelerateDecelerateTransition(duration, finalValue, accelerationRatio, decelerationRatio, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateSmoothStopTransition(UI_ANIMATION_SECONDS duration, double finalValue)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateSmoothStopTransition(duration, finalValue, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateParabolicTransitionFromAcceleration(double finalValue, double finalVelocity, double acceleration)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateParabolicTransitionFromAcceleration(finalValue, finalVelocity, acceleration, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateCubicTransition(UI_ANIMATION_SECONDS maximumDuration, double finalValue, double finalVelocity)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateCubicTransition(maximumDuration, finalValue, finalVelocity, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateReversalTransition(UI_ANIMATION_SECONDS duration)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateReversalTransition(duration, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateSinusoidalTransitionFromRange(UI_ANIMATION_SECONDS duration, double minimumValue, double maximumValue, UI_ANIMATION_SECONDS period, UI_ANIMATION_SLOPE slope)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateSinusoidalTransitionFromRange(duration, minimumValue, maximumValue, period, slope, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateSinusoidalTransitionFromVelocity(UI_ANIMATION_SECONDS duration, UI_ANIMATION_SECONDS period)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateSinusoidalTransitionFromVelocity(duration, period, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateLinearTransitionFromSpeed(double speed, double finalValue)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateLinearTransitionFromSpeed(speed, finalValue, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateLinearTransition(UI_ANIMATION_SECONDS duration, double finalValue)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateLinearTransition(duration, finalValue, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateDiscreteTransition(UI_ANIMATION_SECONDS delay, double finalValue, UI_ANIMATION_SECONDS hold)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateDiscreteTransition(delay, finalValue, hold, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateConstantTransition(UI_ANIMATION_SECONDS duration)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateConstantTransition(duration, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationTransitionPtr Animator::CreateInstantaneousTransition(double finalValue)
{
	IUIAnimationTransitionPtr trans;
	if (pTransLib && SUCCEEDED(pTransLib->CreateInstantaneousTransition(finalValue, &trans)))
		return trans;
	return nullptr;
}

IUIAnimationStoryboardPtr Animator::CreateStoryBoard()
{
	IUIAnimationStoryboardPtr storyBoard;
	if (pAnimMgr && SUCCEEDED(pAnimMgr->CreateStoryboard(&storyBoard)))
		return storyBoard;
	return nullptr;
}

HRESULT Animator::RunStoryBoard(IUIAnimationStoryboardPtr storyBoard, std::function<void()> callback)
{
	if (!storyBoard || !pAnimTmr)
		return E_POINTER;
	// set up the notification handlers and the timer callback function
	if (timerEventHandler)
	{
		NotificationAnimationEventHandler* notificationAnimEvHandler = new NotificationAnimationEventHandler();
		timerEventHandler->AddCallback(storyBoard.GetInterfacePtr(), callback);
		notificationAnimEvHandler->SetTimerObj(timerEventHandler);
		storyBoard->SetStoryboardEventHandler(notificationAnimEvHandler);
	}

	// start the animation
	UI_ANIMATION_SECONDS secs = 0;
	pAnimTmr->GetTime(&secs);
	auto hr = storyBoard->Schedule(secs);

	// If animation timer was deactivated, activate it again
	if (pAnimTmr->IsEnabled() != S_OK)
		pAnimTmr->Enable();
	return hr;
}

HRESULT Animator::AbandonAllStoryBoards()
{
	return pAnimMgr->AbandonAllStoryboards();
}

Animator::Animator()
{
	// Create the IUIAnimationManager.
	if (FAILED(pAnimMgr.CreateInstance(CLSID_UIAnimationManager, 0, CLSCTX_INPROC_SERVER)))
		return;

	if (FAILED(pAnimMgr->SetDefaultLongestAcceptableDelay(0.0)))
		return;

	// Create the IUIAnimationTimer.
	if (FAILED(pAnimTmr.CreateInstance(CLSID_UIAnimationTimer, 0, CLSCTX_INPROC_SERVER)))
		return;

	// Attach the timer to the manager by calling IUIAnimationManager::SetTimerUpdateHandler(),
	// passing an IUIAnimationTimerUpdateHandler. You can get this interface by querying the IUIAnimationTimer.
	IUIAnimationTimerUpdateHandlerPtr pTmrUpdater;
	if (FAILED(pAnimMgr->QueryInterface(IID_PPV_ARGS(&pTmrUpdater))))
		return;

	if (FAILED(pAnimTmr->SetTimerUpdateHandler(pTmrUpdater, UI_ANIMATION_IDLE_BEHAVIOR_DISABLE)))
		return;

	// add the timer event handler: this is a global object that handles all
	// callbacks, but calls the callback functions for the StoryBoards
	timerEventHandler = new CTimerEventHandler();
	pAnimTmr->SetTimerEventHandler(timerEventHandler); // timerEventHandler is AddRef'ed here

	// Create the IUIAnimationTransitionLibrary.
	if (FAILED(pTransLib.CreateInstance(CLSID_UIAnimationTransitionLibrary, 0, CLSCTX_INPROC_SERVER)))
		return;
}

Animator::~Animator()
{
	// release the timer event handler object (CTimerEventHandler)
	if (pAnimTmr)
		pAnimTmr->SetTimerEventHandler(nullptr);
	// shut down the animation manager: No methods can be called on any animation object after Shutdown
	if (pAnimMgr)
		pAnimMgr->Shutdown();
}

Animator& Animator::Instance()
{
	if (!instance)
		instance.reset(new Animator());
	return *instance.get();
}

void Animator::ShutDown()
{
	instance.reset(nullptr);
}

std::unique_ptr<Animator> Animator::instance = nullptr;
