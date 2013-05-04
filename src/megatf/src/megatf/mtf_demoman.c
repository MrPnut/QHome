/*
 *  QWProgs-MegaTF_C
 *  Copyright (C) 2006  XavioR
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
 *  $Id: mtf_demoman.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"
void	GrenadeTouch(  );
void	GrenadeExplode(  );

// MTF Demoman Functions

// Balloon (Air) Mirv
void BalloonMIRVtouch ( )
{
	if ( trap_pointcontents( PASSVEC3( self->s.v.origin ) ) == CONTENT_SKY )
		self->s.v.velocity[2] = 0;

	if ( trap_pointcontents( PASSVEC3( self->s.v.origin ) ) == -2 )
		SetVector( self->s.v.velocity, 0, 0, 0 );
		//self->s.v.velocity = '0 0 0';
}

void BalloonMirvGrenadeLaunch ( vec3_t org, gedict_t *shooter )
{
	float xdir;
	float ydir;
	float zdir;

	xdir = ( 150 * g_random(  ) ) - 75;
	ydir = ( 150 * g_random(  ) ) - 75;
	zdir = 40 * g_random(  );
	newmis = spawn (  );
	newmis->s.v.owner = EDICT_TO_PROG( shooter );
	newmis->s.v.movetype = MOVETYPE_BOUNCE;
	newmis->s.v.solid = SOLID_BBOX;
	newmis->s.v.classname = "grenade";
	newmis->s.v.weapon = 40;
	newmis->s.v.touch = ( func_t ) GrenadeTouch;
	newmis->s.v.think = ( func_t ) GrenadeExplode;
	newmis->s.v.nextthink = g_globalvars.time + 2 + ( g_random(  ) * 2 );
	newmis->s.v.velocity[0] = xdir * 2;
	newmis->s.v.velocity[1] = ydir * 2;
	newmis->s.v.velocity[2] = zdir * 10;
	SetVector( newmis->s.v.avelocity, 0, 0, 0 );
	setmodel( newmis, "progs/hgren2.mdl" );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	setorigin( newmis, PASSVEC3( org ) );
}

void BalloonMIRVpop ( )
{
	float i;
	vec3_t new_org;
	new_org[0] = self->s.v.origin[0];new_org[1] = self->s.v.origin[1];new_org[2] = self->s.v.origin[2] - 1;

	sound( self, 3, "shambler/sboom.wav", 1, 1 );
	mtf_deathmsg( 40 );
	i = 0;
	while ( i < 6 )
	{
		BalloonMirvGrenadeLaunch ( new_org, PROG_TO_EDICT( self->s.v.owner ) );
		i = i + 1;
	}
	BecomeExplosion (  );
}

void BalloonMIRVnoise ( )
{
	self->playerclass = self->playerclass + 1;
	self->s.v.nextthink = g_globalvars.time + 0.3;
	if ( ( self->s.v.velocity[0] < 3 ) && ( self->s.v.velocity[1] < 3 ) )
		sound ( self, 0, "player/axhit2.wav", 1, 1 );
	else
		sound ( self, 0, "weapons/throw.wav", 1, 1 );

	if ( self->playerclass >= 20 )
		self->s.v.think = ( func_t ) BalloonMIRVpop;
}

void BalloonMIRV ( )
{
	if ( self->no_grenades_2 < 1 ) {
		G_sprint ( self, 2, "You need type 2 grens for an AirMIRV\n" );
		return;
	}
	if ( self->last_saveme_sound > g_globalvars.time )
		return;

	self->last_saveme_sound = g_globalvars.time + 2;
	self->no_grenades_2 = self->no_grenades_2 - 1;
	G_sprint ( self, 2, "AirMIRV released!\n" );
	sound ( self, 3, "weapons/chngnu1a.wav", 1, 1 );	
	newmis = spawn (  );
	newmis->s.v.health = 180;
	newmis->s.v.takedamage = 2;
	newmis->th_die = BalloonMIRVpop;
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->s.v.weapon = 40;
	newmis->s.v.movetype = MOVETYPE_FLY;
	newmis->s.v.solid = SOLID_BBOX;//1;
	newmis->s.v.classname = "bot";//"grenade";
	// pablo. netname so i can fudge gun's aim.
	newmis->s.v.netname = "airmirv";
	//
	newmis->playerclass = 0;
	newmis->team_no = self->team_no; // can't shotgun your own airmirvs anyway.

	makevectors ( self->s.v.v_angle );
	if ( self->s.v.v_angle[0] ) {
//		newmis->s.v.velocity = ( g_globalvars.v_forward * 100 ) + ( g_globalvars.v_up * 200 );
		newmis->s.v.velocity[0] =
		    g_globalvars.v_forward[0] * 100 + g_globalvars.v_up[0] * 200;
		newmis->s.v.velocity[1] =
		    g_globalvars.v_forward[1] * 100 + g_globalvars.v_up[1] * 200;
		newmis->s.v.velocity[2] =
		    g_globalvars.v_forward[2] * 100 + g_globalvars.v_up[2] * 200;
	}
	else {
		//newmis->s.v.velocity = aim ( self, 10000 );
		aim( newmis->s.v.velocity );
		//newmis->s.v.velocity = ( newmis->s.v.velocity * 100 );
		newmis->s.v.velocity[0] = newmis->s.v.velocity[0] * 10;
		newmis->s.v.velocity[1] = newmis->s.v.velocity[1] * 10;
		newmis->s.v.velocity[2] = 200;
	}
	SetVector( newmis->s.v.avelocity, 0, 0, 0 );
	setsize ( newmis, -8, -8, -8, 8, 8, 8 );
	setorigin ( newmis, PASSVEC3( self->s.v.origin ) );
	newmis->s.v.nextthink = ( func_t ) ( g_globalvars.time + 1 );
	newmis->s.v.think = ( func_t ) BalloonMIRVnoise;
	newmis->s.v.touch = ( func_t ) BalloonMIRVtouch;
	setmodel ( newmis, "progs/air-mirv.mdl" );
	mtf_deathmsg( 40 );
}
