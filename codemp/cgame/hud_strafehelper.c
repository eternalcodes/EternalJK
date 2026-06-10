/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
Copyright (C) 2013 - 2015, OpenJK contributors
Copyright (C) 2015 - 2021, EternalJK contributors
Copyright (C) 2015 - 2023, TaystJK contributors


This file is part of the TaystJK source code.

TaystJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <https://www.gnu.org/licenses/>.
===========================================================================
*/

#include "hud_strafehelper.h"
#include "cg_local.h"

int DF_GetMovePhysics() {
	int physics = MV_JKA;

	if(cgs.serverMod == SVMOD_JAPRO) {
		if(!cg.demoPlayback) {
			physics = cg.predictedPlayerState.stats[STAT_MOVEMENTSTYLE];
		} else if(cg.snap) {
			physics = cg.snap->ps.stats[STAT_MOVEMENTSTYLE];
		}
	}
	return physics;
}

/* Physics Constants Getters */

//get the movestyle's pm_accelerate - acceleration on ground constant
float DF_GetAccelerate() {
	float accelerate;
	switch (state.moveStyle) {
		case MV_CPM:
		case MV_OCPM:
		case MV_RJCPM:
		case MV_BOTCPM:
			accelerate = pm_cpm_accelerate;
			break;
		case MV_WSW:
			accelerate = pm_wsw_accelerate;
			break;
		case MV_SLICK:
			accelerate = pm_slick_accelerate;
			break;
		case MV_SP:
			accelerate = pm_sp_accelerate;
			break;
		case MV_SURF:
			accelerate = pm_surf_accelerate;
			break;
		case MV_PJK:
		default:
			accelerate = pm_accelerate;
	}
	return accelerate;
}

//get the movestyle's pm_airaccelerate - acceleration in air constant
float DF_GetAirAccelerate() {
	float airAccelerate;
	switch (state.moveStyle) {
		case MV_PJK:
		case MV_CPM:
		case MV_OCPM:
		case MV_WSW:
		case MV_SLICK:
		case MV_RJCPM:
		case MV_BOTCPM:
			airAccelerate = pm_cpm_airaccelerate;
			break;
		case MV_SP:
			airAccelerate = pm_sp_airaccelerate;
			break;
		case MV_QW:
			airAccelerate = pm_qw_airaccelerate;
			break;
		case MV_SURF:
			airAccelerate = pm_surf_airaccelerate;
			break;
		case MV_TRIBES:
			airAccelerate = pm_tribes_airaccelerate;
			break;
		case MV_JETPACK:
		default:
			airAccelerate = pm_airaccelerate;
	}
	return airAccelerate;
}

//get the movestyle's pm_airstrafeaccelerate - acceleration in air with air control constant
float DF_GetAirStrafeAccelerate() {
	float airStrafeAccelerate;
	switch (state.moveStyle) {
		case MV_PJK:
		case MV_CPM:
		case MV_OCPM:
		case MV_WSW:
		case MV_RJCPM:
		case MV_BOTCPM:
			airStrafeAccelerate = pm_cpm_airstrafeaccelerate;
			break;
		case MV_SLICK:
			airStrafeAccelerate = pm_slick_airstrafeaccelerate;
			break;
		case MV_SP:
			airStrafeAccelerate = pm_sp_airaccelerate;
			break;
		default:
			airStrafeAccelerate = pm_airaccelerate;
	}
	return airStrafeAccelerate;
}

//get the movestyle's pm_airstopaccelerate - constant of air accel stop rate with air control
float DF_GetAirStopAccelerate() {
	float airStopAccelerate;
	switch (state.moveStyle) {
		case MV_PJK:
		case MV_CPM:
		case MV_OCPM:
		case MV_WSW:
		case MV_SLICK:
		case MV_RJCPM:
		case MV_BOTCPM:
			airStopAccelerate = pm_cpm_airstopaccelerate;
			break;
		default:
			airStopAccelerate = 0.0f;
		}
	return airStopAccelerate;
}

//get the movestyle's pm_airstrafewishspeed - constant of wishspeed while in air with air control
float DF_GetAirStrafeWishspeed() {
	float airStrafeWishSpeed;
	switch (state.moveStyle) {
		case MV_PJK:
		case MV_CPM:
		case MV_OCPM:
		case MV_WSW:
		case MV_SLICK:
		case MV_RJCPM:
		case MV_BOTCPM:
			airStrafeWishSpeed = pm_cpm_airstrafewishspeed;
			break;
		default:
			airStrafeWishSpeed = 0;
	}
	return airStrafeWishSpeed;
}

//get the movestyle's pm_friction - constant of friction
float DF_GetFriction() {
	float friction;
	switch (state.moveStyle) {
		case MV_CPM:
		case MV_WSW:
		case MV_RJCPM:
		case MV_BOTCPM:
		case MV_SLICK:
			friction = pm_vq3_friction;
			break;
		case MV_OCPM:
		case MV_RJQ3:
		case MV_QW:
		case MV_Q3:
		default:
			friction = pm_friction;
	}
	return friction;
}

//get the movestyle's pm_duckscale - how much speed is scaled when ducking
float DF_GetDuckScale() {
	float duckScale;
	switch (state.moveStyle) {
		case MV_Q3:
		case MV_RJQ3:
			duckScale = pm_vq3_duckScale;
			break;
		case MV_WSW:
			duckScale = pm_wsw_duckScale;
			break;
		default:
			duckScale = pm_duckScale;
	}
	return duckScale;
}

/* Physics Constants Checkers */

//does the style have air control - using A/D turning to turn and accelerate fast in air
qboolean DF_HasAirControl() {
	qboolean hasAirControl;
	switch (state.moveStyle) {
		case MV_PJK:
		case MV_CPM:
		case MV_OCPM:
		case MV_WSW:
		case MV_SLICK:
		case MV_RJCPM:
		case MV_BOTCPM:
			hasAirControl = qtrue;
			break;
		default:
			hasAirControl = qfalse;
	}
	return hasAirControl;
}

qboolean DF_CenterOnly() {
	qboolean centerOnly;
	switch (state.moveStyle) {
		case MV_SURF:
		case MV_TRIBES:
			centerOnly = qtrue;
			break;
		default:
			centerOnly = qfalse;
	}
	return centerOnly;
}

static float DF_GetAirAccelForCmd(const usercmd_t cmd) {
	float accel = state.physics.airaccelerate;
	vec3_t wishvel, forward, right, up;

	if (state.physics.hasAirControl) {
		if (cmd.forwardmove == 0 && cmd.rightmove != 0) {
			accel = state.physics.airstrafeaccelerate;
		}
		else {
			AngleVectors(state.viewAngles, forward, right, up);
			forward[2] = 0;
			right[2] = 0;
			VectorNormalize(forward);
			VectorNormalize(right);

			wishvel[0] = forward[0] * cmd.forwardmove + right[0] * cmd.rightmove;
			wishvel[1] = forward[1] * cmd.forwardmove + right[1] * cmd.rightmove;
			wishvel[2] = 0;

			if (VectorNormalize(wishvel) > 0.0f
				&& DotProduct(state.velocity, wishvel) < 0.0f
				&& state.physics.airstopaccelerate > 0.0f) {
				accel = state.physics.airstopaccelerate;
				}
		}
	}

	if (state.moveStyle == MV_QW) {
		accel *= pm_qw_friction;
	}
	else if (state.moveStyle == MV_SURF) {
		accel *= pm_friction; //PM_CS_AirAccelerate multiplies by the base friction constant
	}
	else if (state.moveStyle == MV_TRIBES) {
		accel *= pm_tribes_airfriction;
	}

	return accel;
}

static float DF_GetAccelWishspeed(const usercmd_t inCmd) {
	if (!state.cgaz.groundMove) {
		if (state.moveStyle == MV_QW) {
			return state.speed; // uncapped wishspeed multiplier for QW air accel
		}
		if (state.moveStyle == MV_SURF) {
			//PM_CS_AirAccelerate's accelspeed term uses the raw cmd magnitude (surf zeroes rightmove on diagonals, so this is always 127), capped at pm_surf_wishspeed
			return 127.0f;
		}
	}

	return DF_GetWishspeed(inCmd);
}

//styles where PM_CmdScale (including upmove) must be emulated instead of the state.speed shortcut
static qboolean DF_StyleUsesCmdScale(void) {
	switch (state.moveStyle) {
		case MV_OCPM:
		case MV_SP:
		case MV_TRIBES:
		case MV_JETPACK:
			return qtrue;
		default:
			return qfalse;
	}
}

static qboolean DF_UseGroundAccel(void) {
	if (!state.cgaz.groundMove) {
		return qfalse;
	}

	if (state.moveStyle == MV_OCPM) {
		return qtrue;
	}

	return state.cgaz.frictionFrame;
}

qboolean showSnapHud() {
	if ((cgs.serverMod == SVMOD_JAPRO && state.moveStyle == MV_OCPM)
		|| (cgs.serverMod == SVMOD_JAPRO && !state.racemode)
		|| cgs.serverMod != SVMOD_JAPRO) {
		return qtrue;
	}
	return qfalse;
}

/* Draw HUD */
void DF_DrawStrafeHUD(centity_t* cent)
{
	//set the playerstate
	if (DF_SetPlayerState(cent) == 1) {
		if( cg_strafeHelper.integer & SHELPER_ORIGINAL ||
			cg_strafeHelper.integer & SHELPER_UPDATED ||
			cg_strafeHelper.integer & SHELPER_CGAZ ||
			cg_strafeHelper.integer & SHELPER_WSW ||
			cg_strafeHelper.integer & SHELPER_ACCELMETER ||
			cg_strafeHelper.integer & SHELPER_WEZE ||
			cg_strafeHelper.integer & SHELPER_ACCELZONES )
		{
			DF_StrafeHelper();
		}
	}

	if (cg_snapHud.integer) {
		if (showSnapHud() == qtrue) {
			DF_DrawSnapHud();
		}
	}

	if (cg_pitchHud.integer) {
		DF_DrawPitchHud(state.viewAngles[PITCH]);
	}

	//japro movement keys
	if (cg_movementKeys.integer) {
		DF_DrawMovementKeys();
	}

	if (cg_speedometer.integer & SPEEDOMETER_ENABLE || cg_strafeHelper.integer & SHELPER_ACCELMETER) {
		DF_SetSpeedometer();
		DF_DrawSpeedometer();

		if (cg_speedometer.integer & SPEEDOMETER_ACCELMETER || cg_strafeHelper.integer & SHELPER_ACCELMETER)
			DF_DrawAccelMeter();
		if (cg_speedometer.integer & SPEEDOMETER_JUMPHEIGHT)
			DF_DrawJumpHeight();
		if (cg_speedometer.integer & SPEEDOMETER_JUMPDISTANCE)
			DF_DrawJumpDistance();
		if (cg_speedometer.integer & SPEEDOMETER_VERTICALSPEED)
			DF_DrawVerticalSpeed();
		if (cg_speedometer.integer & SPEEDOMETER_YAWSPEED)
			DF_DrawYawSpeed();
		if (cg_speedometer.integer & SPEEDOMETER_SPEEDGRAPH) {
			rectDef_c speedgraphRect;
			const vec4_t foreColor = { 0.0f, 0.8f, 1.0f, 0.8f };
			vec4_t backColor = { 0.0f, 0.8f, 1.0f, 0.0f };
			speedgraphRect.x = SCREEN_WIDTH * 0.5f - 150.0f / 2.0f;
			speedgraphRect.y = SCREEN_HEIGHT - 22 - 2;
			speedgraphRect.w = 150.0f;
			speedgraphRect.h = 22.0f;
			DF_GraphAddSpeed();
			DF_DrawSpeedGraph(&speedgraphRect, foreColor, backColor);
		}
		if (cg_speedometer.integer & SPEEDOMETER_SPEEDGRAPHOLD)
			DF_DrawSpeedGraphOld();
	}

	if (cg_raceTimer.integer || cg_raceStart.integer)
		DF_RaceTimer();

	//jaPRO strafehelper line crosshair

	if (cg_strafeHelper.integer & SHELPER_CROSSHAIR) {
		vec4_t hcolor;

		if (!cg.crosshairColor[0] && !cg.crosshairColor[1] && !cg.crosshairColor[2]) { //default to white
			hcolor[0] = 1.0f;
			hcolor[1] = 1.0f;
			hcolor[2] = 1.0f;
			hcolor[3] = 1.0f;
		}
		else {
			hcolor[0] = cg.crosshairColor[0];
			hcolor[1] = cg.crosshairColor[1];
			hcolor[2] = cg.crosshairColor[2];
			hcolor[3] = cg.crosshairColor[3];
		}

		float lineWidth = cg_strafeHelperLineWidth.value;
		if (lineWidth < 0.25f)
			lineWidth = 0.25f;
		else if (lineWidth > 5)
			lineWidth = 5;

		DF_DrawLine(0.5f * SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT + 5.0f,
			0.5f * SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT - 5.0f,
			lineWidth, hcolor, 0); //640x480, 320x240
	}

	//set wasOnGround here, we need this as friction only gets applied if on the ground for more than 1 frame
	if(state.onGround == qtrue) //on ground this frame, jerky with pps
	{
		state.cgaz.wasOnGround = qtrue;
	}
	else {
		state.cgaz.wasOnGround = qfalse;
	}

	cg.lastZSpeed = state.velocity[2];
}

/* Strafehelper */

//main strafehelper function, sets states and then calls drawstrafeline function for each keypress
void DF_StrafeHelper() {
	qboolean checkForW = state.m_iVehicleNum ?
			(qboolean)(state.moveDir == KEY_W) :
			(qboolean)(state.moveDir == KEY_W || state.moveDir == KEY_WA || state.moveDir == KEY_DW);
	//set strafehelper settings struct
	DF_SetStrafeHelper(); //state.strafehelper.


	if(DF_CenterOnly() == qtrue || state.physics.hasAirControl) {
		dfsline centerLine;
		centerLine = DF_GetLine(KEY_CENTER, qfalse, GAZ_CENTER, qfalse);
		if (centerLine.onScreen) {
			DF_DrawStrafeLine(centerLine);
		}
		if(state.strafeHelper.rear) {
			dfsline rearCenterLine;
			rearCenterLine = DF_GetLine(KEY_CENTER, qtrue, GAZ_CENTER, qfalse);
			if (rearCenterLine.onScreen) {
				DF_DrawStrafeLine(rearCenterLine);
			}
		}
	}

	//get the other active gaz lines
	if (!(state.cmd.forwardmove == 0 && state.cmd.rightmove == 0) && DF_CenterOnly() == qfalse) //only do this if keys are pressed
	{
		int i;
		dfsline line, rearLine, minLine, rearMinLine, maxLine, rearMaxLine, maxCosLine, rearMaxCosLine, activeLine, rearActiveLine;

		float activeMin = 0, rearActiveMin = 0, activeOpt = 0, rearActiveOpt = 0, activeMaxCos = 0, rearActiveMaxCos = 0;
		//active min angle
		minLine = DF_GetLine(state.moveDir, qfalse, GAZ_MIN, qfalse);
		if (minLine.onScreen && minLine.active) {
			activeMin = minLine.x;
		}

		//alternate active min angle
		if (state.strafeHelper.rear || checkForW) { //always draw both lines for W
			rearMinLine = DF_GetLine(state.moveDir, qtrue, GAZ_MIN, qfalse); //get the rear max line
			if (rearMinLine.onScreen && rearMinLine.active) {
				rearActiveMin = rearMinLine.x;
			}
		}

		//active opt angle
		activeLine = DF_GetLine(state.moveDir, qfalse, GAZ_OPT, qfalse);
		if (activeLine.onScreen && activeLine.active) {
			activeOpt = activeLine.x;
		}

		//alternative opt angle
		if (state.strafeHelper.rear || checkForW) {
			rearActiveLine = DF_GetLine(state.moveDir, qtrue, GAZ_OPT, qfalse);
			if (rearActiveLine.onScreen && rearActiveLine.active) {
				rearActiveOpt = rearActiveLine.x;
			}
		}

		//active max cosine angle
		maxCosLine = DF_GetLine(state.moveDir, qfalse, GAZ_MAX_COS, qfalse);
		if (maxCosLine.onScreen && maxCosLine.active) {
			activeMaxCos = maxCosLine.x;
		}

		//alternate active max cosine angle
		if (state.strafeHelper.rear || checkForW) { //always draw both lines for W
			rearMaxCosLine = DF_GetLine(state.moveDir, qtrue, GAZ_MAX_COS, qfalse); //get the rear max line
			if (rearMaxCosLine.onScreen && rearMaxCosLine.active) {
				rearActiveMaxCos = rearMaxCosLine.x;
			}
		}
		//active max angle
		maxLine = DF_GetLine(state.moveDir, qfalse, GAZ_MAX, qfalse);

		//alternate active max angle
		if (state.strafeHelper.rear || checkForW) { //always draw both lines for W
			rearMaxLine = DF_GetLine(state.moveDir, qtrue, GAZ_MAX, qfalse); //get the rear max line
		}
		for (i = 0; i < KEY_CENTER; i++) {
			if (i != state.moveDir) { //don't need to recalc it for active
				//normal opt angle
				line = DF_GetLine(i, qfalse, GAZ_OPT, qtrue);
				if (line.onScreen && !line.active) {
					DF_DrawStrafeLine(line);
				}

				//alternate opt angle
				if (state.strafeHelper.rear || i == KEY_W) { //player has rear lines enabled, or it is W
					rearLine = DF_GetLine(i, qtrue, GAZ_OPT, qtrue);
					if (rearLine.onScreen && !rearLine.active) {
						DF_DrawStrafeLine(rearLine);
					}
				}
			}
		}

		if (state.strafeHelper.triangles) {
			//min to opt
			if (minLine.onScreen && minLine.active) {
				if (activeLine.onScreen && activeLine.active) {
					if (state.racemode && state.moveStyle != MV_OCPM)
						DF_DrawTriangle(activeOpt, activeMin);
					else
						DF_DrawZone(activeOpt, activeMin);

				}
			}

			//min to opt rear
			if (state.strafeHelper.rear || checkForW) {
				if (rearMinLine.onScreen && rearMinLine.active) {
					if (rearActiveLine.onScreen && rearActiveLine.active) {
						if (state.racemode && state.moveStyle != MV_OCPM)
							DF_DrawTriangle(rearActiveOpt, rearActiveMin);
						else
							DF_DrawZone(rearActiveOpt, rearActiveMin);
					}
				}
			}

			//opt to max cos
			if (activeLine.onScreen && activeLine.active) {
				if (maxCosLine.onScreen && maxCosLine.active) {
					if (state.racemode && state.moveStyle != MV_OCPM)
						DF_DrawTriangle(activeOpt, activeMaxCos);
					else
						DF_DrawZone(activeOpt, activeMaxCos);

				}
			}

			//opt to max cos rear
			if(state.strafeHelper.rear || checkForW) {
				if (rearActiveLine.onScreen && rearActiveLine.active) {
					if (rearMaxCosLine.onScreen && rearMaxCosLine.active) {
						if (state.racemode && state.moveStyle != MV_OCPM)
							DF_DrawTriangle(rearActiveOpt, rearActiveMaxCos);
						else
							DF_DrawZone(rearActiveOpt, rearActiveMaxCos);
					}
				}
			}
		}

		if(state.strafeHelper.max) {
			//draw the active opt line
			if (maxLine.onScreen && maxLine.active) {
				DF_DrawStrafeLine(maxLine);
			}

			//draw the alternate active opt line
			if ((state.strafeHelper.rear || checkForW)
				&& rearMaxLine.onScreen && rearMaxLine.active) {
				DF_DrawStrafeLine(rearMaxLine);
			}
		}

		//draw the active opt line
		if (activeLine.onScreen && activeLine.active) {
			DF_DrawStrafeLine(activeLine);
		}

		//draw the alternate active opt line
		if ((state.strafeHelper.rear || checkForW)
			&& rearActiveLine.onScreen && rearActiveLine.active) {
			DF_DrawStrafeLine(rearActiveLine);
		}
	}
}

/* Strafehelper Struct Setters */

//sets the dfstate struct used for strafehelper calculations
int DF_SetPlayerState(centity_t* cent)
{
	state.moveStyle = DF_GetMovePhysics();
	state.m_iVehicleNum = cg.predictedPlayerState.m_iVehicleNum;
	state.onGround = (qboolean)(cent->currentState.groundEntityNum != ENTITYNUM_NONE);
	state.groundEntityNum = cent->currentState.groundEntityNum;

	if(state.m_iVehicleNum) {
		state.vehCent = &cg_entities[state.m_iVehicleNum];
		state.velocity = state.vehCent->currentState.pos.trDelta;
	}

	if (cent->currentState.clientNum == cg.clientNum && !cg.demoPlayback) { // are we a real client
		if (!state.m_iVehicleNum) {
			state.velocity = cg.predictedPlayerState.velocity;
		}
		DF_SetClientCmdReal();
	}
	else if (cg.snap) {  //or are we a spectator/demo
		if (!state.m_iVehicleNum) {
			state.velocity = cent->currentState.pos.trDelta;
		}
		DF_SetClientCmd(cent);
	}
	else {
		return 0;
	}

	if (!cg.renderingThirdPerson && !(cg_strafeHelper.integer & SHELPER_ORIGINAL)) {
		VectorCopy(cg.refdef.vieworg, state.viewOrg);
		VectorCopy(cg.refdef.viewangles, state.viewAngles);
	}
	else {
		VectorCopy(cg.predictedPlayerState.origin, state.viewOrg);
		VectorCopy(cg.predictedPlayerState.viewangles, state.viewAngles);
	}
	state.racemode = IsRacemode(&cg.predictedPlayerState);
	state.speed = cg.predictedPlayerState.speed;
	state.commandTime = cg.predictedPlayerState.commandTime;
	state.pm_type = cg.predictedPlayerState.pm_type;
	state.clientnum = cg.predictedPlayerState.clientNum;
	state.vertspeed = state.velocity[2];
	state.pm_time = cg.predictedPlayerState.pm_time;
	state.duelTime = cg.predictedPlayerState.duelTime;
	state.onSlick = DF_IsSlickSurf();
	DF_SetPhysics();
	DF_SetCGAZ();
	return 1;
}

static qboolean CG_IsWalkingAnim(int anim) {
	switch (anim) {
		case BOTH_WALK1:
		case BOTH_WALK2:
		case BOTH_WALK_STAFF:
		case BOTH_WALKBACK_STAFF:
		case BOTH_WALK_DUAL:
		case BOTH_WALKBACK_DUAL:
		case BOTH_WALKBACK1:
		case BOTH_WALKBACK2:
			return qtrue;
		default:
			return qfalse;
	}
}

//sets cmd for a non-predicted client (spectator/demo playback)
void DF_SetClientCmd(centity_t* cent) {
	state.moveDir = cg.snap->ps.movementDir;
	state.cmd = DF_DirToCmd(state.moveDir);
	state.cmd.upmove = 0;
	state.cmd.buttons &= ~(BUTTON_ATTACK | BUTTON_ALT_ATTACK | BUTTON_WALKING);

	if ((DF_GetGroundDistance() > 1 && state.velocity[2] > 8 && state.velocity[2] > cg.lastZSpeed && !cg.snap->ps.fd.forceGripCripple) || cg.snap->ps.pm_flags & PMF_JUMP_HELD)
		state.cmd.upmove = 127;
	else if (cg.snap->ps.pm_flags & PMF_DUCKED || CG_InRollAnim(cent))
		state.cmd.upmove = -1;
	if (sqrtf(state.velocity[0] * state.velocity[0] + state.velocity[1] * state.velocity[1]) < 9)
		state.moveDir = -1;
	if (cent->currentState.eFlags & EF_FIRING && !(cent->currentState.eFlags & EF_ALT_FIRING)) {
		state.cmd.buttons |= BUTTON_ATTACK;
		state.cmd.buttons &= ~BUTTON_ALT_ATTACK;
	}
	else if (cent->currentState.eFlags & EF_ALT_FIRING) {
		state.cmd.buttons |= BUTTON_ALT_ATTACK;
		state.cmd.buttons &= ~BUTTON_ATTACK;
	}
	if (CG_IsWalkingAnim(cent->currentState.legsAnim)) {
		state.cmd.buttons |= BUTTON_WALKING;
	}
}

//sets parts of the dfstate struct for a predicted client
void DF_SetClientCmdReal() {
	state.moveDir = cg.predictedPlayerState.movementDir; //0-7 movement dir
	trap->GetUserCmd(trap->GetCurrentCmdNumber(), &state.cmd);
}

//sets the constants relative to the current movement styles physics
void DF_SetPhysics() {
	state.physics.stopspeed = pm_stopspeed;
	state.physics.duckscale = DF_GetDuckScale();
	state.physics.swimscale = pm_swimScale;
	state.physics.wadescale = pm_wadeScale;
	state.physics.accelerate = DF_GetAccelerate();
	state.physics.airaccelerate = DF_GetAirAccelerate();
	state.physics.wateraccelerate = pm_wateraccelerate;
	state.physics.flightfriction = pm_flightfriction;
	state.physics.friction = DF_GetFriction();
	state.physics.airstopaccelerate = DF_GetAirStopAccelerate();
	state.physics.airstrafewishspeed = DF_GetAirStrafeWishspeed();
	state.physics.airstrafeaccelerate = DF_GetAirStrafeAccelerate();
	state.physics.airdecelrate = pm_sp_airDecelRate;
	state.physics.hasAirControl = DF_HasAirControl();
}

//calls functions that sets values to the cgaz struct
void DF_SetCGAZ() {
	DF_SetFrameTime();
	DF_SetVelocityAngles();

	state.cgaz.groundMove = state.onGround && state.cgaz.wasOnGround;
	state.cgaz.frictionFrame = state.cgaz.groundMove && !state.onSlick;

	state.cgaz.vxyz = sqrtf(state.velocity[0] * state.velocity[0] + state.velocity[1] * state.velocity[1] + state.velocity[2] * state.velocity[2]);

	state.cgaz.s = DF_GetWishspeed(state.cmd);
	state.cgaz.v = sqrtf(state.velocity[0] * state.velocity[0] + state.velocity[1] * state.velocity[1]);

	if (state.cgaz.frictionFrame) {
		const float control = state.cgaz.v < state.physics.stopspeed ? state.physics.stopspeed : state.cgaz.v;
		float vf = state.cgaz.v - control * state.physics.friction * state.cgaz.frametime;
		if (vf < 0) {
			vf = 0;
		}
		state.cgaz.vf = vf;
	} else {
		state.cgaz.vf = state.cgaz.v;
	}

	const float accel = DF_UseGroundAccel() ? state.physics.accelerate : DF_GetAirAccelForCmd(state.cmd);
	state.cgaz.a = DF_GetAccelWishspeed(state.cmd) * accel * state.cgaz.frametime;

	state.cgaz.d_min = CGAZ_Min(state.cgaz.groundMove, state.cgaz.v, state.cgaz.vf, state.cgaz.a, state.cgaz.s);
	state.cgaz.d_opt = CGAZ_Opt(state.cgaz.groundMove, state.cgaz.v, state.cgaz.vf, state.cgaz.a, state.cgaz.s);
	state.cgaz.d_max_cos = CGAZ_Max_Cos(state.cgaz.groundMove, state.cgaz.v, state.cgaz.vf, state.cgaz.a, state.cgaz.s);
	state.cgaz.d_max = CGAZ_Max(state.cgaz.groundMove, state.cgaz.v, state.cgaz.vf, state.cgaz.a, state.cgaz.s);
}

void DF_SetSpeedometer() {
	float currentSpeed;

	speedometerXPos = cg_speedometerX.value;
	jumpsXPos = cg_speedometerJumpsX.value;
	//Offset the speedometer
	if (cgs.newHud) {
		switch (cg_hudFiles.integer) {
			case 0:
				speedometerXPos -= 8;
				break;
			case 1:
				speedometerXPos -= 56;
				break;
			case 2:
				speedometerXPos -= 42;
				break;
			default:
				break;
		}
	}

	if((qboolean)(cg_speedometer.integer & SPEEDOMETER_XYZ))
		currentSpeed = state.cgaz.vxyz;
	else
		currentSpeed = state.cgaz.v;

	if(cg_speedometer.integer & SPEEDOMETER_KPH){
		currentSpeed *= 0.1028699967f;
	} else if (cg_speedometer.integer & SPEEDOMETER_MPH) {
		currentSpeed *= 0.06392043271f;
	} else if(cg_speedometer.integer) {
		currentSpeed = floorf(currentSpeed + 0.5f);
	}
	state.speedometer.speed = currentSpeed;

}

/* CGAZ Setters */

//sets the frametime for the cgaz struct
void DF_SetFrameTime() {
	float frameTime;
	if (cg_strafeHelper_FPS.value < 1) {
		frameTime = (float)cg.frametime/1000;
	}
	else if (cg_strafeHelper_FPS.value > 1000) {
		frameTime = 1.0f / 1000;
	}
	else {
		frameTime = 1.0f / cg_strafeHelper_FPS.value;
	}
	if (frameTime <= 0.0f) {
		frameTime = 0.001f; //1ms floor, equivalent to 1000fps cap
	}
	state.cgaz.frametime = frameTime;
}

//sets the velocity angle for the cgaz struct
void DF_SetVelocityAngles()
{
	vec3_t xyVel = { 0 };
	static vec3_t velAngles = { 0 };
	//set the velocity
	VectorCopy(state.velocity, xyVel);
	xyVel[2] = 0;
	//get the velocity angle
	vectoangles(xyVel, velAngles);
	//set the velocity angle
	VectorCopy(velAngles, state.cgaz.velocityAngles);
}

/* Strafehelper/Line Setters/Getters */

//set the strafehelper user settings to the struct
void DF_SetStrafeHelper() {
	if( state.moveStyle == MV_SIEGE ) {
		return;
	}
	int sensitivity = cg_strafeHelperPrecision.integer;
	const int LINE_HEIGHT = (int)(0.5f * SCREEN_HEIGHT); //240 is midpoint, so it should be a little higher so crosshair is always on it.
	const vec4_t twoKeyColor = { 1, 1, 1, 0.75f }; //WA,WD,SA,SD
	const vec4_t oneKeyColor = { 0.5f, 1, 1, 0.75f }; //A, D
	const vec4_t oneKeyColorAlt = { 1, 0.75f, 0.0f, 0.75f }; //W, S, Center
	const vec4_t rearColor = { 0.75f, 0,1, 0.75f };
	const vec4_t activeColor = { 0, 1, 0, 1.0f };

	//set the default colors
	Vector4Copy(twoKeyColor, state.strafeHelper.twoKeyColor);
	Vector4Copy(oneKeyColor, state.strafeHelper.oneKeyColor);
	Vector4Copy(oneKeyColorAlt, state.strafeHelper.oneKeyColorAlt);
	Vector4Copy(rearColor, state.strafeHelper.rearColor);
	Vector4Copy(activeColor, state.strafeHelper.activeColor);

	//set the line height
	state.strafeHelper.LINE_HEIGHT = LINE_HEIGHT;

	//set the precision/sensitivity
	if (cg_strafeHelperPrecision.integer < 100)
		sensitivity = 100;
	else if (cg_strafeHelperPrecision.integer > 10000)
		sensitivity = 10000;
	state.strafeHelper.sensitivity = sensitivity;

	//set the line width
	float lineWidth = cg_strafeHelperLineWidth.value;
	if (lineWidth < 0.25f)
		lineWidth = 0.25f;
	else if (lineWidth > 5)
		lineWidth = 5;
	state.strafeHelper.lineWidth = lineWidth;

	//set the offset
	state.strafeHelper.offset = cg_strafeHelperOffset.value * 0.01f;

	//is center enabled
	state.strafeHelper.center = qfalse;
	if (cg_strafeHelper.integer & SHELPER_CENTER) {
		state.strafeHelper.center = qtrue;
	}

	state.strafeHelper.rear = qfalse;
	state.strafeHelper.max = qfalse;
	state.strafeHelper.triangles = qfalse;

	if(DF_CenterOnly() == qfalse) {
		if (cg_strafeHelper.integer & SHELPER_REAR) {
			state.strafeHelper.rear = qtrue;
		}

		if (cg_strafeHelper.integer & SHELPER_MAX) {
			state.strafeHelper.max = qtrue;
		}

		if (cg_strafeHelper.integer & SHELPER_ACCELZONES) {
			state.strafeHelper.triangles = qtrue;
		}
	}
}

//Take a moveDir and returns a cmd
usercmd_t DF_DirToCmd(const int moveDir) {
	usercmd_t outCmd;
	memcpy(&outCmd, &state.cmd, sizeof(usercmd_t));
	switch (moveDir) {
	case KEY_W:
		outCmd.forwardmove = 127;
		outCmd.rightmove = 0;
		break;
	case KEY_WA:
		outCmd.forwardmove = 127;
		outCmd.rightmove = -127;
		break;
	case KEY_A:
		outCmd.forwardmove = 0;
		outCmd.rightmove = -127;
		break;
	case KEY_AS:
		outCmd.forwardmove = -127;
		outCmd.rightmove = -127;
		break;
	case KEY_S:
		outCmd.forwardmove = -127;
		outCmd.rightmove = 0;
		break;
	case KEY_SD:
		outCmd.forwardmove = -127;
		outCmd.rightmove = 127;
		break;
	case KEY_D:
		outCmd.forwardmove = 0;
		outCmd.rightmove = 127;
		break;
	case KEY_DW:
		outCmd.forwardmove = 127;
		outCmd.rightmove = 127;
		break;
	default:
		outCmd.forwardmove = 0;
		outCmd.rightmove = 0;
	}
	return outCmd;
}

//get the line struct - big function but no point simplifying it past this state
dfsline DF_GetLine(int moveDir, const qboolean rear, const int gazLine, const qboolean fake) {
	dfsline lineOut = {0}; //the line we will be returning
	qboolean active = qfalse, draw = qfalse;
	float angle = 0;

	//make a fake usercmd for the line we are going to get
	usercmd_t fakeCmd = DF_DirToCmd(moveDir);
	fakeCmd.upmove = state.cmd.upmove; //get the real upmove value
	const float fakeWishspeed = DF_GetWishspeed(fakeCmd); //get the wishspeed for the fake cmd
	const float fakeAccelWishspeed = DF_GetAccelWishspeed(fakeCmd); //get the accelwishspeed for the fake cmd

	//check if the fake command matches the real command, if it does, the line is active (currently pressed)

	if (moveDir != KEY_CENTER) {
		if (state.cmd.rightmove == fakeCmd.rightmove && state.cmd.forwardmove == fakeCmd.forwardmove) {
			active = qtrue;
		} else {
			active = qfalse;
		}
	} else { //center is active when A/D or no keys are pressed
		if ((state.cmd.forwardmove == 0 && state.cmd.rightmove != 0) || (state.cmd.forwardmove == 0 && state.cmd.rightmove == 0)) {
			active = qtrue;
		} else {
			active = qfalse;
		}
	}

	//Here we do some checks that determine the moveDir line should be drawn
	if((DF_CenterOnly() == qtrue && moveDir == KEY_CENTER)
	|| (DF_CenterOnly() == qfalse && moveDir != KEY_CENTER)
	|| (state.strafeHelper.rear && moveDir == KEY_S)
	|| (state.physics.hasAirControl))
	{
		draw = qtrue;
	}

	if(state.m_iVehicleNum){
		if(moveDir == KEY_W || moveDir == KEY_WA || moveDir == KEY_DW){
			if (state.cmd.forwardmove != 0) {
				active = qtrue;
			} else {
				active = qfalse;
			}
			draw = qtrue;
			moveDir = KEY_W;
		} else {
			draw = qfalse;
			active = qfalse;
		}
	}

	//Now we get the angle offset by the key press
	if (moveDir != KEY_CENTER) {
		float delta = 0;
		//center has a fixed location
		//which angle are we getting
		switch (gazLine) {
		case GAZ_MIN:
			delta = state.cgaz.d_min;
			delta += state.strafeHelper.offset;
				break;
		case GAZ_OPT:
			if (fake == qtrue) {
				const float fakeAccel = DF_UseGroundAccel() ? state.physics.accelerate : DF_GetAirAccelForCmd(fakeCmd);

				delta = CGAZ_Opt(state.cgaz.groundMove, state.cgaz.v, state.cgaz.vf,fakeAccelWishspeed * fakeAccel * state.cgaz.frametime, fakeWishspeed);
				delta += state.strafeHelper.offset;
			}
			else {
				delta = state.cgaz.d_opt;
				delta += state.strafeHelper.offset;
			}
			break;
		case GAZ_MAX_COS:
			delta = state.cgaz.d_max_cos;
			delta += state.strafeHelper.offset;
				break;
		case GAZ_MAX:
			delta = state.cgaz.d_max;
			delta += state.strafeHelper.offset;
				break;
		default:
			break;
		}

		//Now we get the angle offset by the key press and users strafehelper setting for that key press/line
		//Each keypress is offset by a difference of pi/4 (45 degrees) to its neighbour(moveDir +/- 1), and the rear
		//angle is offset by pi (180 degrees) away from it's corresponding forward angle, but doing it like this since
		//compiler can optimise it, and it is more readable this way.
		switch (moveDir) {
		case KEY_W:
			if (!(cg_strafeHelper.integer & SHELPER_W)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = 45.0f + delta;
				else
					angle = -45.0f - delta;
			}
			break;
		case KEY_WA:
			if (!(cg_strafeHelper.integer & SHELPER_WA)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = delta;
				else
					angle = -90.0f - delta;
			}
			break;
		case KEY_A:
			if (!(cg_strafeHelper.integer & SHELPER_A)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = -45.0f + delta;
				else
					angle = 225.0f - delta;
			}
			break;
		case KEY_AS:
			if (!(cg_strafeHelper.integer & SHELPER_SA)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = -90 + delta;
				else
					angle = 180.0f - delta;
			}
			break;
		case KEY_S:
			if (!(cg_strafeHelper.integer & SHELPER_S) && !state.strafeHelper.rear) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = 225.0f + delta;
				else
					angle = -225.0f - delta;
			}
			break;
		case KEY_SD:
			if (!(cg_strafeHelper.integer & SHELPER_SD)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = 90.0f - delta;
				else
					angle = 180.0f + delta;
			}
			break;
		case KEY_D:
			if (!(cg_strafeHelper.integer & SHELPER_D)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = 45.0f - delta;
				else
					angle = 135.0f + delta;
			}
			break;
		case KEY_DW:
			if (!(cg_strafeHelper.integer & SHELPER_WD)) {
				draw = qfalse;
			}
			if (draw) {
				if (!rear)
					angle = -delta;
				else
					angle = 90.0f + delta;
			}
			break;
		case KEY_CENTER:
			if (!(cg_strafeHelper.integer & SHELPER_CENTER)) {
				draw = qfalse;
			}
			if(draw) {
				if(!rear)
					angle = 0.0f;
				else
					angle = 180.0f;
			}
			break;
		default:
			break;
		}
	}

	//Get the screen x position of the line
	if (draw) {
		lineOut.active = active;
		lineOut.angle = angle;
		DF_SetAngleToX(&lineOut);
		if (lineOut.onScreen) {
			DF_SetLineColor(&lineOut, moveDir, gazLine);
		}
	}
	return lineOut;
}

//get line x values to pass to drawstrafeine
void DF_SetAngleToX(dfsline* inLine) {
	vec3_t start, angs, forward, delta, line;
	float x = 0, y = 0;
	//get the view angles
	VectorCopy(state.viewOrg, start);
	//get the velocity angles
	VectorCopy(state.cgaz.velocityAngles, angs);
	//set distance from velocity angles to optimum
	angs[YAW] += inLine->angle;
	//get forward angle
	AngleVectors(angs, forward, NULL, NULL);
	VectorCopy(angs, inLine->angs);
	// set the line length
	VectorScale(forward, (float)state.strafeHelper.sensitivity, delta);
	//set the line coords
	line[0] = delta[0] + start[0];
	line[1] = delta[1] + start[1];
	line[2] = start[2];
	// is it on the screen?
	if (!CG_WorldCoordToScreenCoord(line, &x, &y)) {
		inLine->onScreen = qfalse;
	}
	else {
		inLine->onScreen = qtrue;
		inLine->x = x;
		inLine->y = y;
	}
}

//set the color of the line
void DF_SetLineColor(dfsline* inLine, const int moveDir, const int gazLine) {
	vec4_t color = { 1, 1, 1, 0.75f };
	//get the default line color
	Vector4Copy(color, inLine->color);
	//set the colors
	if (inLine->active) {
		if (cg_strafeHelperActiveColor.value) { //does the user have a custom active color set
			color[0] = cg.strafeHelperActiveColor[0];
			color[1] = cg.strafeHelperActiveColor[1];
			color[2] = cg.strafeHelperActiveColor[2];
			color[3] = cg.strafeHelperActiveColor[3];
		}
		else { //make it green
			color[0] = state.strafeHelper.activeColor[0];
			color[1] = state.strafeHelper.activeColor[1];
			color[2] = state.strafeHelper.activeColor[2];
			color[3] = state.strafeHelper.activeColor[3];
		}
		if (gazLine != GAZ_MAX) {
			memcpy(inLine->color, color, sizeof(vec4_t));
		}
		else {
			memcpy(inLine->color, colorRed, sizeof(vec4_t)); //max is always red
		}
	}
	else { //set the other colors
		if (moveDir == KEY_WA || moveDir == KEY_DW) {
			memcpy(inLine->color, state.strafeHelper.twoKeyColor, sizeof(vec4_t));
		}
		else if (moveDir == KEY_A || moveDir == KEY_D) {
			memcpy(inLine->color, state.strafeHelper.oneKeyColor, sizeof(vec4_t));
		}
		else if (moveDir == KEY_W || moveDir == KEY_S || moveDir == KEY_CENTER) {
			memcpy(inLine->color, state.strafeHelper.oneKeyColorAlt, sizeof(vec4_t));
		}
		else if (moveDir == KEY_AS || moveDir == KEY_SD) {
			memcpy(inLine->color, state.strafeHelper.rearColor, sizeof(vec4_t));
		}
		inLine->color[3] = cg_strafeHelperInactiveAlpha.value / 255.0f;
	}
}

// Calculates the minimum CGAZ angle
float CGAZ_Min(const qboolean onGround, const float v, const float vf, float a, const float s) {
	float argument;

	if (onGround == qtrue) {
		// Ensure the expression inside the sqrt is non-negative, and  vf is not zero to avoid division by zero
		if (vf == 0) return 0;

		const float expr = s * s - v * v + vf * vf;

		// Ensure v is not zero to avoid division by zero
		if (expr < 0) return 0;

        argument = sqrtf(expr) / vf;

    } else {
		// Ensure v is not zero to avoid division by zero
		if (v == 0) return 0;

		argument = s / v;
	}

    if (argument < -1.0f) argument = -1.0f;
    if (argument > 1.0f) argument = 1.0f;

	float minDelta = acosf(argument);

	// Convert from radians to degrees
	minDelta = minDelta * (180.0f / M_PI) - 45.0f;

	if (minDelta < 0 || minDelta > 360)
		minDelta = 0;

	return minDelta;
}

// Calculates the optimum CGAZ angle
float CGAZ_Opt(qboolean onGround, float v, const float vf, const float a, const float s) {
	// Ensure vf is not zero to avoid division by zero
	if (vf == 0) return 0;

	float argument = (s - a) / vf;

    if (argument < -1.0f) argument = -1.0f;
    if (argument > 1.0f) argument = 1.0f;

	float optDelta = acosf(argument);

	// Convert from radians to degrees
	optDelta = optDelta * (180.0f / M_PI) - 45.0f;


	if (optDelta < 0 || optDelta > 360)
		optDelta = 0;

	return optDelta;
}

// Calculates the maximum CGAZ angle cosine value (turn angle)
float CGAZ_Max_Cos(const qboolean onGround, const float v, const float vf, const float a, float s) {
	float maxCosDelta;

	if (onGround == qtrue) {
		// Ensure 'a' is not zero to avoid division by zero and undefined behavior
		if (a == 0) return 0;

		float argument = (v - vf) / a;

        if (argument < -1.0f) argument = -1.0f;
        if (argument > 1.0f) argument = 1.0f;

		maxCosDelta = acosf(argument);
	} else {
		// When not on the ground, the max cos delta is set to 90 degrees (pi/2 radians)
		maxCosDelta = 0.5f * M_PI;
	}

	// Convert from radians to degrees
	maxCosDelta = maxCosDelta * (180.0f / M_PI) - 45.0f;

	if (maxCosDelta < 0 || maxCosDelta > 360)
		maxCosDelta = 0;

	return maxCosDelta;
}

// Calculates the maximum CGAZ angle (no more speed gain for current velocity direction)
float CGAZ_Max(const qboolean onGround, const float v, const float vf, const float a, float s) {
	float argument;

	// Ensure 'vf' and 'v' are not zero to avoid division by zero and undefined behavior
	if (2 * a * vf == 0 || 2 * v == 0) return 0;

	if (onGround == qtrue) {
		argument = (v * v - vf * vf - a * a) / (2 * a * vf);
	} else {
		argument = -(a / (2 * v));
	}

	if (argument < -1.0f) argument = -1.0f;
	if (argument > 1.0f) argument = 1.0f;

	float maxDelta = acosf(argument);

	// Convert from radians to degrees
	maxDelta = maxDelta * (180.0f / M_PI) - 45.0f;

	if (maxDelta < 0 || maxDelta > 360)
		maxDelta = 0;

	return maxDelta;
}

//takes a user commmand and returns the emulated wishspeed as a float
float DF_GetWishspeed(const usercmd_t inCmd) {
	vec3_t		wishvel;
	vec3_t		forward, right, up;

	const float fmove = inCmd.forwardmove;
	const float smove = inCmd.rightmove;

	AngleVectors(state.viewAngles, forward, right, up);
	// project moves down to flat plane
	forward[2] = 0;
	right[2] = 0;
	VectorNormalize(forward);
	VectorNormalize(right);

	for (int i = 0; i < 2; i++) {
		wishvel[i] = forward[i] * fmove + right[i] * smove;
	}
	wishvel[2] = 0; //wishdir
	float wishspeed = VectorNormalize(wishvel);
	if (DF_StyleUsesCmdScale())
		wishspeed *= DF_GetCmdScale(inCmd);
	else
		wishspeed = state.speed; //magnitude * PM_CmdScale always cancels to ps->speed for full presses

	//air control styles cap wishspeed when using A or D only in the air (includes OCPM)
	if (!state.cgaz.groundMove && state.physics.hasAirControl &&
		wishspeed > state.physics.airstrafewishspeed && fmove == 0 && smove != 0) {
		wishspeed = state.physics.airstrafewishspeed;
	}

	if (state.moveStyle == MV_SURF) {
		if (!state.cgaz.groundMove) {
			//PM_AirMove never cmd-scales surf, and PM_CS_AirAccelerate caps the addspeed threshold at 30
			wishspeed = 30.0f;
		}
		else if (wishspeed > pm_surf_wishspeed) {
			wishspeed = pm_surf_wishspeed; //PM_WalkMove cap
		}
	}

	if (state.pm_type == PM_JETPACK) {
		if (inCmd.upmove <= 0)
			wishspeed *= 0.8f;
		else
			wishspeed *= 2.0f;
	}
	if (state.m_iVehicleNum) {
		if (state.commandTime < state.vehCent->m_pVehicle->m_iTurboTime) {
			wishspeed = state.vehCent->m_pVehicle->m_pVehicleInfo->turboSpeed;//1400
		}
		else {
			wishspeed = state.vehCent->m_pVehicle->m_pVehicleInfo->speedMax;//700
		}
	}

	if (state.cgaz.groundMove && inCmd.upmove < 0) {
		const float duckWishspeed = state.speed * state.physics.duckscale;

		if (wishspeed > duckWishspeed) {
			wishspeed = duckWishspeed;
		}
	}

	//SP encourages deceleration away from current velocity, air only (PM_AirMove)
	if (state.moveStyle == MV_SP && !state.cgaz.groundMove) {
		if (DotProduct(state.velocity, wishvel) < 0.0f) {
			wishspeed *= state.physics.airdecelrate;
		}
	}

	if (state.moveStyle == MV_QW && !state.cgaz.groundMove) {
		if (wishspeed > pm_qw_airstrafewishspeed) {
			wishspeed = pm_qw_airstrafewishspeed;
		}
	}
	return wishspeed;
}

//takes a user command and returns the emulated command scale as a float
float DF_GetCmdScale(const usercmd_t cmd) {
	signed char		umove = 0; //cmd->upmove;
	//don't factor upmove into scaling speed

	if (DF_StyleUsesCmdScale()) { //upmove velocity scaling
		umove = cmd.upmove;
	}
	int max = abs(cmd.forwardmove);
	if (abs(cmd.rightmove) > max) {
		max = abs(cmd.rightmove);
	}
	if (abs(umove) > max) {
		max = abs(umove);
	}
	if (!max) {
		return 0.0f;
	}

	const float total = sqrtf((float)cmd.forwardmove * (float)cmd.forwardmove
	                           + (float)cmd.rightmove * (float)cmd.rightmove + (float)umove * (float)umove);
	const float scale = state.speed * (float)max / (127.0f * total);

	return scale;
}

/* Strafehelper Style Distributor */

//takes a strafe line and draws it according to the strafehelper style set
void DF_DrawStrafeLine(dfsline line) {

	if (cg_strafeHelper.integer & SHELPER_ORIGINAL) {
		float startx, starty;
		int cutoff = SCREEN_HEIGHT - cg_strafeHelperCutoff.integer; //Should be between 480 and LINE_HEIGHT
		if (cutoff > SCREEN_HEIGHT)
			cutoff = SCREEN_HEIGHT;
		if (cutoff < state.strafeHelper.LINE_HEIGHT)
			cutoff = state.strafeHelper.LINE_HEIGHT;

		if (CG_WorldCoordToScreenCoord(state.viewOrg, &startx, &starty))
			DF_DrawLine(startx - state.strafeHelper.lineWidth / 2.0f, starty, line.x, line.y, state.strafeHelper.lineWidth, line.color, (float)cutoff);
	}

	if (cg_strafeHelper.integer & SHELPER_UPDATED) { //draw the updated style here
		int cutoff = SCREEN_HEIGHT - cg_strafeHelperCutoff.integer;
		int heightIn = state.strafeHelper.LINE_HEIGHT - 10;

		if (cg_strafeHelper.integer & SHELPER_TINY) {
			cutoff = state.strafeHelper.LINE_HEIGHT + 5;
			heightIn = state.strafeHelper.LINE_HEIGHT - 5;
		}
		else if (cutoff < state.strafeHelper.LINE_HEIGHT + 10) {
			cutoff = state.strafeHelper.LINE_HEIGHT + 10;
		}
		else if (cutoff > SCREEN_HEIGHT) {
			cutoff = SCREEN_HEIGHT;
		}
		DF_DrawLine(0.5f * SCREEN_WIDTH, SCREEN_HEIGHT, line.x, (float)heightIn, state.strafeHelper.lineWidth, line.color, (float)cutoff);
	}

	if (cg_strafeHelper.integer & SHELPER_CGAZ) { //draw the cgaz style strafehelper
		if (cg_strafeHelperCutoff.integer > state.strafeHelper.LINE_HEIGHT) {
			DF_DrawLine(line.x, 0.5f * SCREEN_HEIGHT + 5.0f, line.x, 0.5f * SCREEN_HEIGHT - 5.0f, state.strafeHelper.lineWidth, line.color, 0); //maximum cutoff
		}
		else {
			DF_DrawLine(line.x, 0.5f * SCREEN_HEIGHT + 20.0f - cg_strafeHelperCutoff.value / 16.0f, line.x, 0.5f * SCREEN_HEIGHT - 20.0f + cg_strafeHelperCutoff.value / 16.0f, state.strafeHelper.lineWidth, line.color, 0); //default/custom cutoff
		}
	}

	if (cg_strafeHelper.integer & SHELPER_WSW && line.active) { //draw the wsw style strafehelper, not sure how to deal with multiple lines for W only so we don't draw any, the proper way is to tell which line we are closest to aiming at and display the strafehelper for that
		const float width = -4.444f * AngleSubtract(state.viewAngles[YAW], line.angs[YAW]);
		CG_FillRect(0.5f * SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT, width, 12, colorTable[CT_RED]);
	}

	if (cg_strafeHelper.integer & SHELPER_WEZE && line.active) { //call the weze style strafehelper function
		DF_DrawStrafehelperWeze(state.moveDir);
	}

	if (cg_strafeHelper.integer & SHELPER_SOUND && line.active) { //strafehelper sounds - don't do them for the center line, since it's not really a strafe
		DF_StrafeHelperSound(100 * AngleSubtract(state.viewAngles[YAW], line.angs[YAW]));
	}
}

/* Drawing Functions */

//draws a line on the screen
void DF_DrawLine(float x1, float y1, float x2, const float y2, const float size, vec4_t color, float ycutoff) {
	float length = sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

	if (length < 1)
		length = 1;
	else if (length > 2000)
		length = 2000;
	if (ycutoff == 0)
		ycutoff = SCREEN_HEIGHT;

	x1 -= 0.5f * size;
	x2 -= 0.5f * size;
	const float stepx = (x2 - x1) / (length / size);
	const float stepy = (y2 - y1) / (length / size);

	trap->R_SetColor(color);

	for (int i = 0; i <= (int)(length / size); i++) {
		if (x1 < SCREEN_WIDTH && y1 < SCREEN_HEIGHT && y1 < ycutoff)
			CG_DrawPic(x1, y1, size, size, cgs.media.whiteShader);
		x1 += stepx;
		y1 += stepy;
	}
	trap->R_SetColor(NULL);
}

//draws the weze strafehelper
void DF_DrawStrafehelperWeze(const int moveDir) {
	vec3_t velocity_copy;
	vec3_t viewangle_copy;
	vec3_t velocity_angle;

	const float midx = 0.5f * SCREEN_WIDTH;
	const float midy = 0.5f * SCREEN_HEIGHT;
	VectorCopy(state.velocity, velocity_copy);
	velocity_copy[2] = 0;
	VectorCopy(state.viewAngles, viewangle_copy);
	viewangle_copy[PITCH] = 0;
	float length = VectorNormalize(velocity_copy);
	const usercmd_t cmd = DF_DirToCmd(moveDir);

	const float g_speed = state.speed;
	float accel = g_speed;
	accel *= state.cgaz.frametime;
	float optiangle = (g_speed - accel) / length;
	if (optiangle <= 1 && optiangle >= -1)
		optiangle = acosf(optiangle);
	else
		optiangle = 0;
	length /= 5;
	if (length > 0.5f * SCREEN_HEIGHT)
		length = 0.5f * SCREEN_HEIGHT;
	vectoangles(velocity_copy, velocity_angle);
	float diff = AngleSubtract(viewangle_copy[YAW], velocity_angle[YAW]);
	diff = diff / 180.0f * M_PI;

	DF_DrawLine(midx, midy, midx + length * sinf(diff), midy - length * cosf(diff), 1, colorRed, 0);
	DF_DrawLine(midx, midy, midx + (float)cmd.rightmove, midy - (float)cmd.forwardmove, 1, colorCyan, 0);
	DF_DrawLine(midx, midy, midx + length / 2 * sinf(diff + optiangle), midy - length / 2 * cosf(diff + optiangle), 1, colorRed, 0);
	DF_DrawLine(midx, midy, midx + length / 2 * sinf(diff - optiangle), midy - length / 2 * cosf(diff - optiangle), 1, colorRed, 0);
}

//plays the strafehelper sounds
void DF_StrafeHelperSound(const float difference) {
	if (difference > -40.0f && difference < 10.0f) //Under aiming by a bit, but still good
		trap->S_StartLocalSound(cgs.media.hitSound4, CHAN_LOCAL_SOUND);
}

//sets the color of the triangles based on accel - code repeated from speedometer
void DF_SetAccelColor() {
	const float currentSpeed = state.cgaz.v;
	static float lastSpeed = 0, previousAccels[ACCEL_SAMPLES];
	const float accel = currentSpeed - lastSpeed;
	static unsigned int index;
	static int lastupdate;
	vec4_t color = { 0,0,0,0 };

	lastSpeed = currentSpeed;
	const int t = trap->Milliseconds();

	if (t - lastupdate > 5)	//don't sample faster than this
	{
		lastupdate = t;
		previousAccels[index % ACCEL_SAMPLES] = accel;
		index++;
	}

	float total = 0;
	for (int i = 0; i < ACCEL_SAMPLES; i++) {
		total += previousAccels[i];
	}
	if (!total) {
		total = 1;
	}

	const float avgAccel = total / (float) ACCEL_SAMPLES - 0.0625f;//fucking why does it offset by this number

	if (avgAccel > 0.0f) //good accel = green
	{
		color[0] = 0;
		color[1] = 1.0f;
		color[2] = 0;
		color[3] = 0.4f;
	}
	else if (avgAccel < 0.0f) //bad accel = red
	{
		color[0] = 1.0f;
		color[1] = 0;
		color[2] = 0;
		color[3] = 0.4f;
	}
	else //no accel = white
	{
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 0.4f;
	}
	trap->R_SetColor(NULL);
	trap->R_SetColor(color);
}

//draws the acceleration zone triangle
void DF_DrawTriangle(const float start, const float end) {
	if (start <= SCREEN_WIDTH && start >= 0 && end <= SCREEN_WIDTH && end >= 0) {
		DF_SetAccelColor();
		CG_DrawPic(end, 0.5f * SCREEN_HEIGHT - 4.0f, start - end, 8.0f, cgs.media.leftTriangle);
		trap->R_SetColor(NULL); //set the color back to null
	}
}

void DF_DrawZone(const float start, const float end) {
	if (start <= SCREEN_WIDTH && start >= 0 && end <= SCREEN_WIDTH && end >= 0) {
		DF_SetAccelColor();
		CG_DrawPic(end, 0.5f * SCREEN_HEIGHT - 4.0f, start - end, 8.0f, cgs.media.solidWhite);
		trap->R_SetColor(NULL); //set the color back to null
	}
}

void CG_AddSpeedGraphFrameInfo(void) {  //CG_DrawSpeedGraph
	speedgraph.frameSamples[speedgraph.frameCount & (SPEED_SAMPLES - 1)] = (int)state.cgaz.v;
	speedgraph.frameCount++;
}

/*  Speedometer */

/*
===================
CG_GraphAddSpeed
tremulous - append a speed to the sample history for the speed graph
===================
*/
void DF_GraphAddSpeed(void) {
	vec3_t vel;

	if(!oldestSpeedSample){
		oldestSpeedSample = 0;
	}

	if(!maxSpeedSample){
		maxSpeedSample = 0;
	}

	VectorCopy(state.velocity, vel);

	const float speed = VectorLength(vel);

	if (speed > speedSamples[maxSpeedSample])
	{
		maxSpeedSample = oldestSpeedSample;
		speedSamples[oldestSpeedSample++] = speed;
		oldestSpeedSample %= SPEEDOMETER_NUM_SAMPLES;
		return;
	}

	speedSamples[oldestSpeedSample] = speed;
	if (maxSpeedSample == oldestSpeedSample++) // if old max was overwritten find a new one
	{
		int i;
		for (maxSpeedSample = 0, i = 1; i < SPEEDOMETER_NUM_SAMPLES; i++)
		{
			if (speedSamples[i] > speedSamples[maxSpeedSample])
				maxSpeedSample = i;
		}
	}

	oldestSpeedSample %= SPEEDOMETER_NUM_SAMPLES;
}

/*
===================
CG_DrawSpeedGraph
tremulous
===================
*/
void DF_DrawSpeedGraph(const rectDef_c* rect, const vec4_t foreColor, const vec4_t backColor) {
	// color of graph is interpolated between these values
	const vec3_t fast = { 1.0f, 0.0f, 0.0f };
	vec4_t color;

	float max = speedSamples[maxSpeedSample];
	if (max < SPEEDOMETER_MIN_RANGE)
		max = SPEEDOMETER_MIN_RANGE;

	trap->R_SetColor(backColor);
	CG_DrawPic(rect->x, rect->y, rect->w, rect->h, cgs.media.whiteShader);

	Vector4Copy(foreColor, color);

	for (int i = 1; i < SPEEDOMETER_NUM_SAMPLES; i++)
	{
		const vec3_t medium = { 0.0f, 1.0f, 0.0f };
		const vec3_t slow = { 0.0f, 0.0f, 1.0f };
		const float val = speedSamples[(oldestSpeedSample + i) % SPEEDOMETER_NUM_SAMPLES];
		if (val < SPEED_MED)
			VectorLerp(val / SPEED_MED, slow, medium, color);
		else if (val < SPEED_FAST)
			VectorLerp((val - SPEED_MED) / (SPEED_FAST - SPEED_MED),
				medium, fast, color);
		else
			VectorCopy(fast, color);
		trap->R_SetColor(color);
		const float top = rect->y + (1 - val / max) * rect->h;
		CG_DrawPic(rect->x + (float)i / (float)SPEEDOMETER_NUM_SAMPLES * rect->w, top,
			rect->w / (float)SPEEDOMETER_NUM_SAMPLES, val * rect->h / max,
			cgs.media.whiteShader);
	}
	trap->R_SetColor(NULL);
}

void DF_DrawSpeedGraphOld(void) {
	float ah; // mid, range;
	//	int		color;

	float x = SCREEN_WIDTH - (float) cg_lagometerX.integer * cgs.widthRatioCoef;
	float y = SCREEN_HEIGHT - cg_lagometerY.integer - 56;

	if (cg_hudFiles.integer == 0) {
		y -= 16;
	}

	trap->R_SetColor(NULL);

	if (cg_lagometer.integer == 1 || cg_lagometer.integer == 2)
		CG_DrawPic(x, y, 48 * cgs.widthRatioCoef, 48, cgs.media.lagometerShader);

	if (cg_lagometer.integer == 2 || (cg_lagometer.integer == 3 && state.cgaz.v != 0))
		CG_Text_Paint(x + 2 * cgs.widthRatioCoef, y, 0.5f, colorWhite, va("%.0f", state.cgaz.v), 0, 0, ITEM_TEXTSTYLE_SHADOWEDMORE, FONT_SMALL);

	if (cg_lagometer.integer != 1 && cg_lagometer.integer != 2)
		y -= 96;

	x -= 1.0f * cgs.widthRatioCoef;

	const float ax = x;
	const float ay = y;
	const int aw = 48;
	if (cg_lagometer.integer != 1 && cg_lagometer.integer != 2)
		ah = 48 * 3;
	else
		ah = 48;

	// draw the speed graph
	const float range = ah;
	const float vscale = range / 3000;

	for (int a = 0; a < aw; a++) {
		 const int i = (speedgraph.frameCount - 1 - a) & (SPEED_SAMPLES - 1);
		float v = (float) speedgraph.frameSamples[i];
		if (v > 0) {
			trap->R_SetColor(g_color_table[ColorIndex(COLOR_GREEN)]);
			v = v * vscale;
			if (v > range)
				v = range;
			trap->R_DrawStretchPic(ax + (float)(aw - a) * cgs.widthRatioCoef, ay + ah - v, 1 * cgs.widthRatioCoef, v, 0, 0, 0, 0, cgs.media.whiteShader);
		}
	}
	trap->R_SetColor(NULL);
}

/*
===================
CG_DrawJumpHeight
japro - Draw speedometer jump height
===================
*/
void DF_DrawJumpHeight() {
	const vec_t* const velocity = state.velocity;
	char jumpHeightStr[32] = { 0 };

	if (cg.predictedPlayerState.fd.forceJumpZStart == -65536) //Coming back from a tele or w/e
		return;

	if (cg.predictedPlayerState.fd.forceJumpZStart && cg.lastZSpeed > 0 && velocity[2] <= 0) {//If we were going up, and we are now going down, print our height.
		cg.lastJumpHeight = cg.predictedPlayerState.origin[2] - cg.predictedPlayerState.fd.forceJumpZStart;
		cg.lastJumpHeightTime = cg.time;
	}

	if (cg.lastJumpHeightTime > cg.time - 1500 && cg.lastJumpHeight > 0.0f) {
		Com_sprintf(jumpHeightStr, sizeof(jumpHeightStr), "%.1f", cg.lastJumpHeight);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, (float)cg_speedometerY.integer, cg_speedometerSize.value, colorTable[CT_WHITE], jumpHeightStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}

	speedometerXPos += 42;
}

/*
===================
CG_DrawJumpDistance
japro - Draw speedometer jump distance
===================
*/
void DF_DrawJumpDistance(void) {
	char jumpDistanceStr[64] = { 0 };

	if (state.onGround) {

		if (!state.cgaz.wasOnGround) {//We were just in the air, but now we arnt
			vec3_t distance;

			VectorSubtract(state.viewOrg, cg.lastGroundPosition, distance);
			cg.lastJumpDistance = sqrtf(distance[0] * distance[0] + distance[1] * distance[1]);
			cg.lastJumpDistanceTime = cg.time;
		}
		VectorCopy(state.viewOrg, cg.lastGroundPosition);
	}

	if (cg.lastJumpDistanceTime > cg.time - 1500 && cg.lastJumpDistance > 0.0f) {
		Com_sprintf(jumpDistanceStr, sizeof(jumpDistanceStr), "%.1f", cg.lastJumpDistance);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, (float)cg_speedometerY.integer, cg_speedometerSize.value, colorTable[CT_WHITE], jumpDistanceStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}

	speedometerXPos += 62;
}

/*
===================
CG_DrawVerticalSpeed
japro - Draw speedometer vertical speed
===================
*/
void DF_DrawVerticalSpeed(void) {
	char speedStr5[64] = { 0 };
	float vertspeed = state.vertspeed;

	if (vertspeed < 0)
		vertspeed = -vertspeed;

	if (vertspeed) {
		Com_sprintf(speedStr5, sizeof(speedStr5), "%.0f", vertspeed);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, (float)cg_speedometerY.integer, cg_speedometerSize.value, colorWhite, speedStr5, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}

	speedometerXPos += 42;
}

void DF_DrawYawSpeed(void) {
	static unsigned short previousYaws[YAW_FRAMES];
	static unsigned short index;
	static int    lastupdate; //previous, lastupdate;
	//    unsigned short frameTime;
	//    const int        xOffset = 0;

	const float diff = AngleSubtract(state.viewAngles[YAW], cg.lastYawSpeed);
	float yawspeed = diff / state.cgaz.frametime;
	if (yawspeed < 0)
		yawspeed = -yawspeed;

	const int t = trap->Milliseconds();
	//    frameTime = t - previous;
	//    previous = t;
	if (t - lastupdate > 20)    //don't sample faster than this
	{
		lastupdate = t;
		previousYaws[index % YAW_FRAMES] = (int)yawspeed;
		index++;
	}

	int total = 0;
	for (int i = 0; i < YAW_FRAMES; i++) {
		total += previousYaws[i];
	}
	if (!total) {
		total = 1;
	}
	const int yaw = (int) ((float) total / (float) YAW_FRAMES);

	if (yaw) {
		char yawStr[64] = { 0 };
		if (yawspeed > 320)
			Com_sprintf(yawStr, sizeof(yawStr), "^1%03i", (int)((float)yaw + 0.5f)); //added 8 whitespaces idk how much to add - fixme
		else if (yawspeed > 265)
			Com_sprintf(yawStr, sizeof(yawStr), "^3%03i", (int)((float)yaw + 0.5f)); //added 8 whitespaces idk how much to add - fixme
		else
			Com_sprintf(yawStr, sizeof(yawStr), "%03i", (int)((float)yaw + 0.5f)); //added 8 whitespaces idk how much to add - fixme
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, (float)cg_speedometerY.integer, cg_speedometerSize.value, colorTable[CT_WHITE], yawStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}

	cg.lastYawSpeed = state.viewAngles[YAW];

	speedometerXPos += 16;
}

/*
===================
CG_DrawAccelMeter
japro - Draw acceleration meter
===================
*/
void DF_DrawAccelMeter(void) {
	const float optimalAccel = state.speed * state.cgaz.frametime;
	const float potentialSpeed = sqrtf(cg.previousSpeed * cg.previousSpeed - optimalAccel * optimalAccel + 2 * (state.cgaz.s * optimalAccel));
	const float accel = state.cgaz.v - cg.previousSpeed;
	static float previousTimes[PERCENT_SAMPLES];
	static unsigned int index;

	float x = speedometerXPos;

	if (cg_speedometer.integer & SPEEDOMETER_GROUNDSPEED)
		x -= 88;
	else
		x -= 52;

	CG_DrawRect((x - 0.75f) * cgs.widthRatioCoef,
		cg_speedometerY.value - 10.75f,
		37.75f * cgs.widthRatioCoef,
		13.75f,
		0.5f,
		colorTable[CT_BLACK]);

	float actualAccel = accel;
	if (actualAccel < 0)
		actualAccel = 0.001f;
	else if (actualAccel > potentialSpeed - state.cgaz.v) //idk how
		actualAccel = (potentialSpeed - state.cgaz.v) * 0.99f;

	previousTimes[index % PERCENT_SAMPLES] = actualAccel / (potentialSpeed - state.cgaz.v);
	index++;

	float total = 0;
	for (int i = 0; i < PERCENT_SAMPLES; i++) {
		total += previousTimes[i];
	}
	if (!total) {
		total = 1;
	}
	const float percentAccel = total / (float) PERCENT_SAMPLES;

	if (percentAccel && state.cgaz.v) {
		CG_FillRect((x + 0.25f) * cgs.widthRatioCoef,
			cg_speedometerY.value - 9.9f,
			36 * percentAccel * cgs.widthRatioCoef,
			12,
			colorTable[CT_RED]);
	}
	cg.previousSpeed = state.cgaz.v;
}

/*
===================
CG_DrawSpeedometer
japro - Draw the speedometer
===================
*/
 void DF_DrawSpeedometer(void) {
    const char* accelStr, * accelStr2, * accelStr3;
    char speedStr[32] = { 0 }, speedStr2[32] = { 0 }, speedStr3[32] = { 0 };
    vec4_t colorSpeed = { 1, 1, 1, 1 };
	const float currentSpeed = state.speedometer.speed;
	static float lastSpeed = 0, previousAccels[ACCEL_SAMPLES];
    int i;
    static unsigned int index;
    static int lastupdate, jumpsCounter = 0;
    static qboolean clearOnNextJump = qfalse;

	const float accel = currentSpeed - lastSpeed;
    lastSpeed = currentSpeed;

	if (currentSpeed > state.cgaz.s && !(cg_speedometer.integer & SPEEDOMETER_COLORS))
	{
		const float currentSpeedColor = 1 / (currentSpeed / state.cgaz.s * (currentSpeed / state.cgaz.s));
		colorSpeed[1] = currentSpeedColor;
		colorSpeed[2] = currentSpeedColor;
	}

	const int t = trap->Milliseconds();

	if (t - lastupdate > 5)	//don't sample faster than this
	{
		lastupdate = t;
		previousAccels[index % ACCEL_SAMPLES] = accel;
		index++;
	}

	float total = 0;
	for (i = 0; i < ACCEL_SAMPLES; i++) {
		total += previousAccels[i];
	}
	if (!total) {
		total = 1;
	}
	const float avgAccel = total / (float) ACCEL_SAMPLES - 0.0625f;//fucking why does it offset by this number

	if (avgAccel > 0.0f)
	{
		accelStr = S_COLOR_GREEN "\xb5:";
		accelStr2 = S_COLOR_GREEN "k:";
		accelStr3 = S_COLOR_GREEN "m: ";
	}
	else if (avgAccel < 0.0f)
	{
		accelStr = S_COLOR_RED "\xb5:";
		accelStr2 = S_COLOR_RED "k:";
		accelStr3 = S_COLOR_RED "m: ";
	}
	else
	{
		accelStr = S_COLOR_WHITE "\xb5:";
		accelStr2 = S_COLOR_WHITE "k:";
		accelStr3 = S_COLOR_WHITE "m: ";
	}

	if (!(cg_speedometer.integer & SPEEDOMETER_KPH) && !(cg_speedometer.integer & SPEEDOMETER_MPH))
	{
		Com_sprintf(speedStr, sizeof(speedStr), "   %.0f", state.speedometer.speed);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorWhite, accelStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorSpeed, speedStr, 0.0f, 0, ITEM_ALIGN_LEFT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}
	else if (cg_speedometer.integer & SPEEDOMETER_KPH)
	{
		Com_sprintf(speedStr2, sizeof(speedStr2), "   %.0f", state.speedometer.speed);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorWhite, accelStr2, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorSpeed, speedStr2, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}
	else if (cg_speedometer.integer & SPEEDOMETER_MPH)
	{
		Com_sprintf(speedStr3, sizeof(speedStr3), "   %.0f", state.speedometer.speed);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorWhite, accelStr3, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
		CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorSpeed, speedStr3, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
	}
	speedometerXPos += 52;

	if (cg_speedometer.integer & SPEEDOMETER_GROUNDSPEED || (cg_speedometer.integer && cg_speedometer.integer & SPEEDOMETER_JUMPS)) {
		char speedStr4[32] = { 0 };
		char speedsStr4[32] = { 0 };

		vec4_t colorGroundSpeed = { 1, 1, 1, 1 };
		vec4_t colorGroundSpeeds = { 1, 1, 1, 1 };

		if (state.groundEntityNum != ENTITYNUM_NONE || state.velocity[2] < 0) { //On ground or Moving down
			cg.firstTimeInAir = qfalse;
		}
		else if (!cg.firstTimeInAir) { //Moving up for first time
			cg.firstTimeInAir = qtrue;
			cg.lastGroundSpeed = state.speedometer.speed;
			cg.lastGroundTime = cg.time;
			if (cg_speedometer.integer & SPEEDOMETER_JUMPS) {

				if (clearOnNextJump == qtrue) {
					memset(&cg.lastGroundSpeeds, 0, sizeof(cg.lastGroundSpeeds));
					jumpsCounter = 0;
					clearOnNextJump = qfalse;
				}
				cg.lastGroundSpeeds[jumpsCounter++] = cg.lastGroundSpeed; //add last ground speed to the array
			}
		}

		if (cg_speedometer.integer & SPEEDOMETER_JUMPS) {
			if ((state.groundEntityNum != ENTITYNUM_NONE &&
				state.pm_time <= 0 && state.cgaz.v < state.cgaz.s) || state.cgaz.v == 0) {
				clearOnNextJump = qtrue;
			}
			if (cg_speedometerJumps.value && jumpsCounter < cg_speedometerJumps.integer) {
				//if we are in the first n jumps
				for (i = 0; i <= cg_speedometerJumps.integer; i++) { //print the jumps
					const float groundSpeedsColor = 1 / (cg.lastGroundSpeeds[i] / state.cgaz.s * (cg.lastGroundSpeeds[i] / state.cgaz.s));
					Com_sprintf(speedsStr4, sizeof(speedsStr4), "%.0f", cg.lastGroundSpeeds[i]); //create the string
					if (cg_speedometer.integer & SPEEDOMETER_JUMPSCOLORS1) { //color the string
						colorGroundSpeeds[1] = groundSpeedsColor;
						colorGroundSpeeds[2] = groundSpeedsColor;
					}
					else if (cg_speedometer.integer & SPEEDOMETER_JUMPSCOLORS2) {
						if ((jumpsCounter > 0 && cg.lastGroundSpeeds[i] > cg.lastGroundSpeeds[i - 1]) ||
							(i == 0 && cg.lastGroundSpeeds[0] > firstSpeed)) {
							colorGroundSpeeds[0] = groundSpeedsColor;
							colorGroundSpeeds[1] = 1;
							colorGroundSpeeds[2] = groundSpeedsColor;
						}
						else {
							colorGroundSpeeds[0] = 1;
							colorGroundSpeeds[1] = groundSpeedsColor;
							colorGroundSpeeds[2] = groundSpeedsColor;
						}
					}
					if (strcmp(speedsStr4, "0") != 0) {
						CG_Text_Paint(jumpsXPos * cgs.widthRatioCoef, cg_speedometerJumpsY.value,
							cg_speedometerSize.value, colorGroundSpeeds, speedsStr4, 0.0f, 0,
							ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE); //print the jump
						jumpsXPos += 52; //shift x
					}
				}
			}
			else if (cg_speedometerJumps.value &&
				jumpsCounter == cg_speedometerJumps.integer) { //we out of the first n jumps
				firstSpeed = cg.lastGroundSpeeds[0];
				for (i = 0; i <= cg_speedometerJumps.integer; i++) { //shuffle jumps array down
					cg.lastGroundSpeeds[i] = cg.lastGroundSpeeds[i + 1];
				}
				jumpsCounter--;  //reduce jump counter
			}
		}

		const float groundSpeedColor = 1 / (cg.lastGroundSpeed / state.cgaz.s * cg.lastGroundSpeed / state.cgaz.s);
		if (cg_jumpGoal.value && cg_jumpGoal.value <= cg.lastGroundSpeed && jumpsCounter == 1) {
			colorGroundSpeed[0] = groundSpeedColor;
			colorGroundSpeed[1] = 1;
			colorGroundSpeed[2] = groundSpeedColor;
		}
		else if (cg.lastGroundSpeed > state.cgaz.s && !(cg_speedometer.integer & SPEEDOMETER_COLORS)) {
			colorGroundSpeed[0] = 1;
			colorGroundSpeed[1] = groundSpeedColor;
			colorGroundSpeed[2] = groundSpeedColor;
		}

		if (cg.lastGroundTime > cg.time - 1500 && cg_speedometer.integer & SPEEDOMETER_GROUNDSPEED) {
			if (cg.lastGroundSpeed) {
				Com_sprintf(speedStr4, sizeof(speedStr4), "%.0f", cg.lastGroundSpeed);
				CG_Text_Paint(speedometerXPos * cgs.widthRatioCoef, cg_speedometerY.value, cg_speedometerSize.value, colorGroundSpeed, speedStr4, 0.0f, 0, ITEM_ALIGN_LEFT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
			}
		}
		speedometerXPos += 52;
	}
}

/*  Movement Keys */

/*
===================
DF_GetGroundDistance
japro - Ground Distance function for use in jump detection for movement keys
===================
*/
float DF_GetGroundDistance(void) {
	trace_t tr;
	vec3_t start, down;

	VectorCopy(state.viewOrg, start);

	if (!cg.renderingThirdPerson && !(cg_strafeHelper.integer & SHELPER_ORIGINAL)) {
		start[2] -= cg.predictedPlayerState.viewheight;
	}

	VectorCopy(start, down);
	down[2] -= 4096;
	CG_Trace(&tr, start, NULL, NULL, down, state.clientnum, MASK_SOLID);
	VectorSubtract(start, tr.endpos, down);

	return VectorLength(down) - 24.0f;
}

/*
===================
DF_IsSlickSurf
japro - Slick surface detection
===================
*/
qboolean DF_IsSlickSurf(void) {
	trace_t tr;
	vec3_t down;
	qboolean onSlick = qfalse;

	VectorCopy(state.viewOrg, down);
	down[2] -= 128;
	CG_Trace(&tr, state.viewOrg, NULL, NULL, down, state.clientnum, MASK_SOLID);

	if ((state.groundEntityNum != ENTITYNUM_NONE && tr.surfaceFlags & SURF_SLICK)
	|| (state.moveStyle == MV_SLICK && !(state.cmd.buttons & BUTTON_WALKING))
	|| (state.moveStyle == MV_TRIBES && (state.cmd.buttons & BUTTON_DASH))
	|| (cg.predictedPlayerState.pm_flags & PMF_TIME_KNOCKBACK))
		onSlick = qtrue;

	return onSlick;
}

/*
===================
CG_DrawMovementKeys
japro - Draw the movement keys
===================
*/
void DF_DrawMovementKeys() {
	float w, h, x, y;

	//set positions based on which setting is used
	if (cg_movementKeys.integer == 1) {
		w = 16 * cg_movementKeysSize.value * cgs.widthRatioCoef;
		h = 16 * cg_movementKeysSize.value;
		x = SCREEN_WIDTH * 0.5f + cg_movementKeysX.value - w * (cg_movementKeysWalk.integer ? 1.0f : 1.5f);
		y = SCREEN_HEIGHT * 0.9f + cg_movementKeysY.value - h * 1.0f;
	}
	else if (cg_movementKeys.integer == 2) {
		w = 16 * cg_movementKeysSize.value * cgs.widthRatioCoef;
		h = 16 * cg_movementKeysSize.value;
		x = SCREEN_WIDTH * 0.5f + cg_movementKeysX.value - w * (cg_movementKeysWalk.integer ? 1.5f : 2.0f);
		y = SCREEN_HEIGHT * 0.9f + cg_movementKeysY.value - h * 1.0f;
	}
	else if (cg_movementKeys.integer == 3) {
		w = 6 * cg_movementKeysSize.value * cgs.widthRatioCoef;
		h = 6 * cg_movementKeysSize.value;
		x = 0.5f * SCREEN_WIDTH - w * 1.5f;
		y = 0.5f * SCREEN_HEIGHT - h * 1.5f;
	}
	else if (cg_movementKeys.integer == 4) {
		w = 12 * cg_movementKeysSize.value * cgs.widthRatioCoef;
		h = 12 * cg_movementKeysSize.value;
		x = 0.5f * SCREEN_WIDTH + cg_movementKeysX.value - w * 1.5f;
		y = 0.9f * SCREEN_HEIGHT + cg_movementKeysY.value - h * 1.5f;
	}
	else {
		return;
	}

	//draw the keys
	if (cg_movementKeys.integer == 3 || cg_movementKeys.integer == 4) { //new movement keys style
		if (state.cmd.upmove < 0)
			CG_DrawPic(w * 2 + x, y, w, h, cgs.media.keyCrouchOnShader2);
		if (state.cmd.upmove > 0)
			CG_DrawPic(x, y, w, h, cgs.media.keyJumpOnShader2);
		if (state.cmd.forwardmove < 0)
			CG_DrawPic(w + x, h * 2 + y, w, h, cgs.media.keyBackOnShader2);
		if (state.cmd.forwardmove > 0)
			CG_DrawPic(w + x, y, w, h, cgs.media.keyForwardOnShader2);
		if (state.cmd.rightmove < 0)
			CG_DrawPic(x, h + y, w, h, cgs.media.keyLeftOnShader2);
		if (state.cmd.rightmove > 0)
			CG_DrawPic(w * 2 + x, h + y, w, h, cgs.media.keyRightOnShader2);
		if (state.cmd.buttons & BUTTON_ATTACK)
			CG_DrawPic(x, 2 * h + y, w, h, cgs.media.keyAttackOn2);
		if (state.cmd.buttons & BUTTON_ALT_ATTACK)
			CG_DrawPic(w * 2 + x, 2 * h + y, w, h, cgs.media.keyAltOn2);
		if (cg_movementKeysWalk.integer) {
			if (state.cmd.buttons & BUTTON_WALKING)
				CG_DrawPic(x - w, 2 * h + y, w, h, cgs.media.keyWalkOn2);
		}
	}
	else if (cg_movementKeys.integer == 1 || cg_movementKeys.integer == 2) { //original movement keys style
		if (state.cmd.upmove < 0)
			CG_DrawPic(w * 2 + x, y, w, h, cgs.media.keyCrouchOnShader);
		else
			CG_DrawPic(w * 2 + x, y, w, h, cgs.media.keyCrouchOffShader);
		if (state.cmd.upmove > 0)
			CG_DrawPic(x, y, w, h, cgs.media.keyJumpOnShader);
		else
			CG_DrawPic(x, y, w, h, cgs.media.keyJumpOffShader);
		if (state.cmd.forwardmove < 0)
			CG_DrawPic(w + x, h + y, w, h, cgs.media.keyBackOnShader);
		else
			CG_DrawPic(w + x, h + y, w, h, cgs.media.keyBackOffShader);
		if (state.cmd.forwardmove > 0)
			CG_DrawPic(w + x, y, w, h, cgs.media.keyForwardOnShader);
		else
			CG_DrawPic(w + x, y, w, h, cgs.media.keyForwardOffShader);
		if (state.cmd.rightmove < 0)
			CG_DrawPic(x, h + y, w, h, cgs.media.keyLeftOnShader);
		else
			CG_DrawPic(x, h + y, w, h, cgs.media.keyLeftOffShader);
		if (state.cmd.rightmove > 0)
			CG_DrawPic(w * 2 + x, h + y, w, h, cgs.media.keyRightOnShader);
		else
			CG_DrawPic(w * 2 + x, h + y, w, h, cgs.media.keyRightOffShader);
		if (cg_movementKeys.integer == 2) {
			if (state.cmd.buttons & BUTTON_ATTACK)
				CG_DrawPic(w * 3 + x, y, w, h, cgs.media.keyAttackOn);
			else
				CG_DrawPic(w * 3 + x, y, w, h, cgs.media.keyAttackOff);
			if (state.cmd.buttons & BUTTON_ALT_ATTACK)
				CG_DrawPic(w * 3 + x, h + y, w, h, cgs.media.keyAltOn);
			else
				CG_DrawPic(w * 3 + x, h + y, w, h, cgs.media.keyAltOff);
		}
		if (cg_movementKeysWalk.integer) {
			if (state.cmd.buttons & BUTTON_WALKING)
				CG_DrawPic(x - w, h + y, w, h, cgs.media.keyWalkOn);
			else
				CG_DrawPic(x - w, h + y, w, h, cgs.media.keyWalkOff);
		}
	}
}

void DF_RaceTimer(void)
{
	if (!state.racemode || !state.duelTime) {
		cg.startSpeed = 0;
		cg.displacement = 0;
		cg.maxSpeed = 0;
		cg.displacementSamples = 0;
		return;
	}

	{
		char timerStr[48] = { 0 };
		char startStr[48] = { 0 };
		vec4_t colorStartSpeed = { 1, 1, 1, 1 };

		const int time = cg.time - state.duelTime;
		const int minutes = time / 1000 / 60;
		const int seconds = time / 1000 % 60;
		const int milliseconds = time % 1000;

		if (time < cg.lastRaceTime) {
			cg.startSpeed = 0;
			cg.displacement = 0;
			cg.maxSpeed = 0;
			cg.displacementSamples = 0;
		}

		if (cg_raceTimer.integer > 1 || cg_raceStart.integer) {
			if (time > 0) {
				if (!cg.startSpeed)
					cg.startSpeed = (int)state.cgaz.v;
				if (state.cgaz.v > (float)cg.maxSpeed)
					cg.maxSpeed = (int)state.cgaz.v;
				cg.displacement += (int)state.cgaz.v;
				cg.displacementSamples++;
			}
		}

		cg.lastRaceTime = time;
		if (cg_raceTimer.integer != 0) {
			if (cg_raceTimer.integer < 3)
				Com_sprintf(timerStr, sizeof(timerStr), "%i:%02i.%i\n", minutes, seconds, milliseconds / 100);
			else
				Com_sprintf(timerStr, sizeof(timerStr), "%i:%02i.%03i\n", minutes, seconds, milliseconds);

			if (cg_raceTimer.integer > 1) {
				if (cg.displacementSamples)
					Q_strcat(timerStr, sizeof(timerStr), va("Max: %i\nAvg: %i", (int)((float)cg.maxSpeed + 0.5f),
						cg.displacement / cg.displacementSamples));
				if (time < 3000 && !cg_raceStart.integer)
					Q_strcat(timerStr, sizeof(timerStr), va("\nStart: %i", cg.startSpeed));

			}

			CG_Text_Paint((float)cg_raceTimerX.integer * cgs.widthRatioCoef, (float)cg_raceTimerY.integer, cg_raceTimerSize.value, colorTable[CT_WHITE],
				timerStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
		}
		if (cg_raceStart.integer)
		{
			if (cg_startGoal.value && cg_startGoal.value <= (float)cg.startSpeed) {
				const float startColor = 1 / ((float)cg.startSpeed / state.cgaz.s * ((float)cg.startSpeed / state.cgaz.s));
				colorStartSpeed[0] = startColor;
				colorStartSpeed[1] = 1;
				colorStartSpeed[2] = startColor;
			}
			Com_sprintf(startStr, sizeof(startStr), "Start: %i", cg.startSpeed);
			CG_Text_Paint((float)cg_raceStartX.integer * cgs.widthRatioCoef, (float)cg_raceStartY.integer, cg_raceTimerSize.value, colorStartSpeed, startStr, 0.0f, 0, ITEM_ALIGN_RIGHT | ITEM_TEXTSTYLE_OUTLINED, FONT_NONE);
		}
	}
}

void DF_DrawShowPos(void)
{
	static char showPosString[128];
	static char showPitchString[8];
	vec4_t colorPitch = { 0, 0, 0, 1 };
	float pitchAngle = fabsf(state.viewAngles[PITCH] + cg_pitchHelperOffset.value);
	if (pitchAngle > 90) {
		pitchAngle = 90;
	}
	const float pitchRange = cg_pitchHelperRange.value / 2;
	if (cg_pitchHelper.value && pitchAngle < pitchRange) {
		float pitchColor = pitchAngle / pitchRange;
		colorPitch[0] = pitchColor;
		pitchColor = 1.0f - pitchAngle / pitchRange / 2.0f;
		colorPitch[1] = pitchColor;
	}
	else if (cg_pitchHelper.value) {
		colorPitch[0] = 1;
		colorPitch[1] = 0;
	}

	if (cg_pitchHelper.value) {
		Com_sprintf(showPitchString, sizeof(showPitchString), "%.1f", (float)state.viewAngles[PITCH]);
		CG_Text_Paint((float)cg_pitchHelperX.integer, (float)cg_pitchHelperY.integer, 1.0f, colorPitch, showPitchString, 0, 0, ITEM_TEXTSTYLE_OUTLINESHADOWED, FONT_SMALL2);
	}

	if (!cg_showpos.integer)
		return;

	const float vel = sqrtf(state.cgaz.v * state.cgaz.v + state.velocity[2] * state.velocity[2]);

	Com_sprintf(showPosString, sizeof(showPosString), "pos:   %.2f   %.2f   %.2f\nang:   %.2f   %.2f\nvel:     %.2f",
		(float)state.viewOrg[0], (float)state.viewOrg[1], (float)state.viewOrg[2], (float)state.viewAngles[PITCH], (float)state.viewAngles[YAW], vel);

	CG_Text_Paint(SCREEN_WIDTH - (SCREEN_WIDTH - 340) * cgs.widthRatioCoef, 0, 0.6f, colorWhite,
		showPosString, 0, 0, ITEM_TEXTSTYLE_OUTLINESHADOWED, FONT_SMALL2);
}

/* SnapHUD */

/*
================
DF_FillAngleYaw
=================
*/
void DF_FillAngleYaw(const float start, const float end, const float viewangle, const float y, const float height, const float* color) {
	const vec2_t cgamefov = { cg.refdef.fov_x, cg.refdef.fov_y };

	const float fovscale = tanf(DEG2RAD(cgamefov[0] / 2));
	const float x = 0.5f * SCREEN_WIDTH + tanf(DEG2RAD(viewangle + start)) / fovscale * (0.5f * SCREEN_WIDTH);
	const float width = fabsf(SCREEN_WIDTH * (tanf(DEG2RAD(viewangle + end)) - tanf(DEG2RAD(viewangle + start))) / (
		                    fovscale * 2)) + 1;

	trap->R_SetColor(color);
	trap->R_DrawStretchPic(x, y, width, height, 0, 0, 0, 0, cgs.media.whiteShader);
	trap->R_SetColor(NULL);
}

static int QDECL sortzones(const void* a, const void* b) {
	return (int)(*(float*)a - *(float*)b);
}

void DF_UpdateSnapHudSettings(float speed, const int fps, const qboolean pro) {
	state.snappinghud.promode = pro;
	state.snappinghud.fps = fps;
	state.snappinghud.speed = speed;
	speed /= (float)state.snappinghud.fps;
	state.snappinghud.count = 0;

	float step = floorf(speed + 0.5f) - 0.5f;
	while (step > 0 && state.snappinghud.count < SNAPHUD_MAXZONES - 2) {
		state.snappinghud.zones[state.snappinghud.count] = RAD2DEG(acosf(step / speed));
		state.snappinghud.count++;
		state.snappinghud.zones[state.snappinghud.count] = RAD2DEG(asinf(step / speed));
		state.snappinghud.count++;
		step -= 1.0f;
	}

	qsort(state.snappinghud.zones, state.snappinghud.count, sizeof(state.snappinghud.zones[0]), sortzones);
	state.snappinghud.zones[state.snappinghud.count] = state.snappinghud.zones[0] + 90;
}

/*
==============
DF_DrawSnapHud
==============
*/
void DF_DrawSnapHud(void)
{
	vec2_t va;
	vec4_t	color[3];
	int fps;
	int colorid = 0;
	qboolean pro = qfalse;

	va[YAW] = state.viewAngles[YAW];
	state.snappinghud.m[0] = state.cmd.forwardmove;
	state.snappinghud.m[1] = state.cmd.rightmove;

	const float speed = cg_snapHudSpeed.value ? cg_snapHudSpeed.value : state.cgaz.s;
	if (cgs.serverMod == SVMOD_JAPRO && state.moveStyle == MV_OCPM) {
		fps = 125;
	}
	else {
		fps = cg_snapHudFps.integer ? cg_snapHudFps.integer : com_maxFPS.integer;
	}

	if (state.physics.hasAirControl == qtrue) {
		pro = qtrue;
	}

	if (speed != state.snappinghud.speed || fps != state.snappinghud.fps || pro != state.snappinghud.promode) {
		DF_UpdateSnapHudSettings(speed, fps, pro);
	}

	const float y = cg_snapHudY.value;
	const float h = cg_snapHudHeight.value;

	switch (cg_snapHudAuto.integer) {
	case 0:
		va[YAW] += cg_snapHudDef.value;
		break;
	case 1:
		if (state.snappinghud.promode || (state.snappinghud.m[0] != 0 && state.snappinghud.m[1] != 0)) {
			va[YAW] += 45;
		}
		else if (state.snappinghud.m[0] == 0 && state.snappinghud.m[1] == 0) {
			va[YAW] += cg_snapHudDef.value;
		}
		break;
	case 2:
		if (state.snappinghud.m[0] != 0 && state.snappinghud.m[1] != 0) {
			va[YAW] += 45;
		}
		else if (state.snappinghud.m[0] == 0 && state.snappinghud.m[1] == 0) {
			va[YAW] += cg_snapHudDef.value;
		}
		break;
	default:
		return;
	}

	color[1][0] = cg.snapHudRgba2[0];
	color[1][1] = cg.snapHudRgba2[1];
	color[1][2] = cg.snapHudRgba2[2];
	color[1][3] = cg.snapHudRgba2[3];

	color[0][0] = cg.snapHudRgba1[0];
	color[0][1] = cg.snapHudRgba1[1];
	color[0][2] = cg.snapHudRgba1[2];
	color[0][3] = cg.snapHudRgba1[3];

	for (int i = 0; i < state.snappinghud.count; i++) {
		DF_FillAngleYaw(state.snappinghud.zones[i], state.snappinghud.zones[i + 1], va[YAW], y, h, color[colorid]);
		DF_FillAngleYaw(state.snappinghud.zones[i] + 90, state.snappinghud.zones[i + 1] + 90, va[YAW], y, h, color[colorid]);
		colorid ^= 1;
	}
}

/* PitchHUD */

/*
==============
DF_MarkAnglePitch
==============
*/
void DF_MarkAnglePitch(const float angle, const float height, const float viewangle, const float x, const float width, const float* color) {
	const vec2_t cgamefov = { cg.refdef.fov_x, cg.refdef.fov_y };

	if (-state.viewAngles[PITCH] + angle > cgamefov[1] / 2 + 5) return;
	const float fovscale = tanf(DEG2RAD(cgamefov[1] / 2));
	const float y = 0.5f * SCREEN_HEIGHT + tanf(DEG2RAD(viewangle + angle)) / fovscale * (0.5f * SCREEN_HEIGHT);

	trap->R_SetColor(color);
	trap->R_DrawStretchPic(x - 0.5f * width, y - 0.5f * height, width, height, 0, 0, 0, 0, cgs.media.whiteShader);
	trap->R_SetColor(NULL);
}

/*
==============
DF_DrawPitchHud
==============
*/
void DF_DrawPitchHud(const float pitch) {
	vec4_t	color;

	color[0] = cg.pitchHudRgba[0];
	color[1] = cg.pitchHudRgba[1];
	color[2] = cg.pitchHudRgba[2];
	color[3] = cg.pitchHudRgba[3];

	const float angle = cg_pitchHudAngle.value;

	DF_MarkAnglePitch(angle, cg_pitchHudThickness.value, -pitch, cg_pitchHudX.value, cg_pitchHudWidth.value, color);
}