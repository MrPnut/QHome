/*
 *  QWProgs-TF2003
 *  Copyright (C) 2004  [sd] angel
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  $Id: sniper.c,v 1.4 2005/05/16 06:31:38 AngelD Exp $
 */
#include "g_local.h"

void	AirStrike_Target ( vec3_t org );
void    SniperSight_Update(  );

//=========================================================================
// Sniper/Auto Rifle selection function
void TeamFortress_SniperWeapon(  )
{

	self->s.v.impulse = 0;
	if ( self->tfstate & TFSTATE_RELOADING )
		return;
	if ( !( ( self->weapons_carried & WEAP_SNIPER_RIFLE ) && ( self->weapons_carried & WEAP_AUTO_RIFLE ) ) )
		return;
	if ( self->s.v.ammo_shells < 1 )
	{
		G_sprint( self, 2, "not enough ammo.\n" );
		return;
	}
	if ( self->current_weapon == WEAP_SNIPER_RIFLE )
		self->current_weapon = WEAP_AUTO_RIFLE;
	else
		self->current_weapon = WEAP_SNIPER_RIFLE;
	W_SetCurrentAmmo(  );
}

//=========================================================================
// Do the autozoom of the sniper rifle
void TF_zoom( int zoom_level )
{

	if ( self->tfstate & TFSTATE_ZOOMOFF )
		return;
	stuffcmd( self, "fov %d\n", zoom_level );
}

//=========================================================================
// Move the sight to the point the rifle's aiming at
void SniperSight_Update(  )
{
	vec3_t  org, end, t_one, t_two;
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( !( owner->tfstate & TFSTATE_AIMING ) || owner->current_weapon != 32 )
	{
		owner->tfstate -= ( owner->tfstate & TFSTATE_AIMING );
		TeamFortress_SetSpeed( owner );
		owner->heat = 0;
		dremove( self );
		return;
	}
	makevectors( owner->s.v.v_angle );
	org[0] = owner->s.v.origin[0] + g_globalvars.v_forward[0] * 10;
	org[1] = owner->s.v.origin[1] + g_globalvars.v_forward[1] * 10;
	org[2] = owner->s.v.absmin[2] + owner->s.v.size[2] * 0.7;

	VectorScale( g_globalvars.v_forward, 9192, end );
	VectorAdd( end, org, end );
	traceline( PASSVEC3( org ), PASSVEC3( end ), 0, self );
	if ( g_globalvars.trace_fraction == 1 )
	{
		setorigin( self, PASSVEC3( owner->s.v.origin ) );
		return;
	}
	vectoangles( g_globalvars.v_forward, self->s.v.angles );
	setorigin( self, PASSVEC3( g_globalvars.trace_endpos ) );
	// airstrike!
	if ( owner->option == 9 ) {
		t_one[0] = self->s.v.origin[0];t_one[1] = self->s.v.origin[1];t_one[2] = self->s.v.origin[2] + 5;
		t_two[0] = self->s.v.origin[0];t_two[1] = self->s.v.origin[1];t_two[2] = self->s.v.origin[2] + 2048;
		traceline( PASSVEC3( t_one ), PASSVEC3( t_two ), 0, self);
		if ( trap_pointcontents( PASSVEC3( g_globalvars.trace_endpos ) ) == CONTENT_SKY ) {
			if ( owner->last_used > g_globalvars.time ) {
				G_sprint( owner, 2, "Reinforcements not in attack position yet... wait 2 minutes\n" );
				owner->option = 0;
			}
			else {
				sound( owner, 0, "weapons/radioair.wav", 1, 1 );
				owner->last_used = ( g_globalvars.time + 120 );
				owner->option = 0;
				AirStrike_Target( t_one );
			}
		}
		else {
			G_sprint( owner, 2, "Target NOT visible from air!\n" );
			if ( owner->last_saveme_sound < g_globalvars.time ) {
				sound( owner, 0, "weapons/notarget.wav", 1, 1 );
				owner->last_saveme_sound = ( g_globalvars.time + 3 );
			}
			owner->option = 0;
		}
	}
	self->s.v.nextthink = g_globalvars.time + 0.1;
}

//=========================================================================
// Create the sight
void SniperSight_Create(  )
{
	gedict_t *sight;

	if ( self->has_disconnected == 1 )
		return;
	self->tfstate |= TFSTATE_AIMING;
	sight = spawn(  );
	sight->s.v.owner = EDICT_TO_PROG( self );
	sight->s.v.movetype = MOVETYPE_NOCLIP;
	sight->s.v.solid = SOLID_NOT;
//	setmodel( sight, "progs/sight.spr" );
	if ( self->option4 != 1 )
	{
		if ( self->team_no == 1 )
		{
			setmodel ( sight, "progs/dot1.spr" );
		}
		else
		{
			if ( self->team_no == 3 )
			{
				setmodel ( sight, "progs/dot3.spr" );
			}
			else
			{
				if ( self->team_no == 4 )
				{
					setmodel ( sight, "progs/dot4.spr" );
				}
				else
				{
					setmodel ( sight, "progs/dot2.spr" );
				}
			}
		}
	}
	sight->s.v.classname = "timer";
	setorigin( sight, PASSVEC3( self->s.v.origin ) );
	sight->s.v.think = ( func_t ) SniperSight_Update;
	sight->s.v.nextthink = g_globalvars.time + 0.05;
}

// this toggles the snipers autozoom on/off
void TeamFortress_AutoZoomToggle(  )
{
	if ( self->tfstate & TFSTATE_ZOOMOFF )
	{
		self->tfstate = self->tfstate - TFSTATE_ZOOMOFF;
		G_sprint( self, 2, "autozoom ON\n" );
	} else
	{
		self->tfstate = self->tfstate | TFSTATE_ZOOMOFF;
		G_sprint( self, 2, "autozoom OFF\n" );
	}
}
