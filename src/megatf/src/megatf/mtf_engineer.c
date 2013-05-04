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
 *  $Id: mtf_engineer.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// MegaTF engineer functions
void	DroneDie();
void	DroneJet();
void	BalloonMIRVtouch();
// Laser Drone (drop3)
void Drone ( )
{
	vec3_t set_org;

	if ( self->no_grenades_1 < 1 || self->s.v.ammo_cells < 100 ) {
		G_sprint( self, 2, "You need 1 Normal Grenade & 100 Armor\n" );
		return;
	}
	if ( self->last_used > g_globalvars.time ) {
		G_sprint( self, 2, "Not finished building new drone yet... takes 3 minutes\n" );
		return;
	}

	G_sprint ( self, 2, "Laser Drone away!\n" );
	self->last_used = g_globalvars.time + 180;
	self->no_grenades_1 -= 1;
	self->s.v.ammo_cells -= 100;
	newmis = spawn (  );
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->s.v.weapon = 41;
	newmis->s.v.movetype = MOVETYPE_FLYMISSILE;
	newmis->s.v.solid = SOLID_BBOX;
	newmis->s.v.classname = "bot";
	newmis->team_no = self->team_no;

	makevectors( self->s.v.v_angle );
	if ( self->s.v.v_angle[0] ) {
		newmis->s.v.velocity[0] =
			g_globalvars.v_forward[0] * 200;
		newmis->s.v.velocity[1] =
			g_globalvars.v_forward[1] * 200;
		newmis->s.v.velocity[2] =
			g_globalvars.v_forward[2] * 200;
	}
	else {
		aim( newmis->s.v.velocity );

		newmis->s.v.velocity[0] =
			newmis->s.v.velocity[0] * 200;
		newmis->s.v.velocity[1] =
			newmis->s.v.velocity[1] * 200;
		newmis->s.v.velocity[2] =
			newmis->s.v.velocity[2] * 200;
	}

	newmis->option= 1;
	newmis->s.v.health = 30;
	newmis->s.v.takedamage = 2;
	newmis->th_die = DroneDie;
	setsize( newmis, -8, -8, -8, 12, 12, 12 );

	// do origin work
	set_org[0] =
		( self->s.v.origin[0] + ( g_globalvars.v_forward[0] * 8 ) ) + 0;
	set_org[1] =
		( self->s.v.origin[1] + ( g_globalvars.v_forward[1] * 8 ) ) + 0;
	set_org[2] =
		( self->s.v.origin[2] + ( g_globalvars.v_forward[2] * 8 ) ) + 20;
//	setorigin( newmis, ( ( self->s.v.origin + ( v_forward * 8 ) ) + '0 0 20' ) );
	setorigin( newmis, PASSVEC3( set_org ) );

	newmis->s.v.angles[0] = self->s.v.angles[0];
	newmis->s.v.angles[1] = self->s.v.angles[1];
	newmis->s.v.angles[2] = self->s.v.angles[2];
	sound ( newmis, 0, "weapons/chngnu1a.wav", 1, 1 );
	newmis->s.v.nextthink = g_globalvars.time + 0.8;
	newmis->playerclass = 0;
	newmis->s.v.think = ( func_t ) DroneJet;
	newmis->s.v.touch = ( func_t ) BalloonMIRVtouch;
	newmis->s.v.skin = 0;
	setmodel( newmis, "progs/lsrdrone.mdl" );
}

void Drone_Laser_Touch ( )
{
	//float deflect;
	vec3_t org, vtemp;
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( other == owner )
		return;
	if ( trap_pointcontents( PASSVEC3( self->s.v.origin ) ) == CONTENT_SKY ) {
		dremove ( self );
		return;
	}
//	if ( ( other->s.v.playerclass == 7 ) )
//	{
//		deflect = hwguy_deflect ( other );
//		if ( deflect )
//		{
//			return;
//		}
//	}
	sound( self, 1, "enforcer/enfstop.wav", 1, 3 );
	normalize( self->s.v.velocity, vtemp );
	org[0] =
		self->s.v.origin[0] - ( 8 * vtemp[0] );
	org[1] =
		self->s.v.origin[0] - ( 8 * vtemp[1] );
	org[2] =
		self->s.v.origin[0] - ( 8 * vtemp[2] );
	//org = ( self->s.v.origin - ( 8 * normalize ( self->s.v.velocity ) ) );
	if ( other->s.v.health ) {
		SpawnBlood ( org, 15 );
		mtf_deathmsg( 41 );
		self->s.v.weapon = 41;
		if ( other->s.v.classname == "building_sentrygun" )
			T_RadiusDamage ( self, PROG_TO_EDICT( owner->s.v.owner ), 90, world );
		else
			T_Damage ( other, self, PROG_TO_EDICT( owner->s.v.owner ), 25 );
	}
	dremove ( self );
}

void DroneLaunchLaser ( vec3_t org, vec3_t vec )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	sound( self, 1, "enforcer/enfire.wav", 1, 1 );
	normalize ( vec, vec );
	newmis = spawn (  );
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->team_no = self->team_no;
	newmis->s.v.movetype = MOVETYPE_FLY;
	newmis->s.v.solid = SOLID_BBOX;
	newmis->s.v.effects = EF_DIMLIGHT;
	newmis->s.v.weapon = 41;
	newmis->s.v.classname = "drone_laser";
	newmis->s.v.skin = ( owner->team_no - 1 );
	setmodel( newmis, "progs/beam.mdl" );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	setorigin ( newmis, PASSVEC3( org ) );

	newmis->s.v.velocity[0] =
		vec[0] * 2000;
	newmis->s.v.velocity[1] =
		vec[1] * 2000;
	newmis->s.v.velocity[2] =
		vec[2] * 2000;
	//newmis->s.v.velocity = ( vec * 2000 );
	vectoangles( newmis->s.v.velocity, newmis->s.v.angles );
	newmis->s.v.nextthink = g_globalvars.time + 4;
	newmis->s.v.think = ( func_t ) SUB_Remove;
	newmis->s.v.touch = ( func_t ) Drone_Laser_Touch;
}

void DroneDoneTouch ( )
{
	if ( self->s.v.velocity[2] )
		sound ( self, 1, "weapons/bounce.wav", 1, 1 );
}

void DroneDie ( )
{
	ThrowGib( "progs/tgib2.mdl", -70 );
	ThrowGib( "progs/dgib2.mdl", -70 );
	ThrowGib( "progs/dgib3.mdl", -70 );
	trap_WriteByte( MSG_MULTICAST, 23 );
	trap_WriteByte( MSG_MULTICAST, 3 );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
	trap_multicast( PASSVEC3( self->s.v.origin ), MULTICAST_PHS );
	BecomeExplosion (  );
}

void DroneFall ( )
{
	sound( self, 0, "weapons/chngnu1a.wav", 1, 1 );
	self->s.v.movetype = MOVETYPE_BOUNCE;
	SetVector( self->s.v.avelocity, 200, 0, 300 );
	//self->s.v.avelocity = '200 0 300';
	setsize ( self, -8, -8, -8, 8, 8, 8 );
	self->s.v.takedamage = 2;
	self->s.v.skin = 1;
	self->th_die = DroneDie;
	self->s.v.touch = ( func_t ) DroneDoneTouch;
	self->s.v.think = ( func_t )  DroneDie;
	self->s.v.nextthink = g_globalvars.time + 20;
}

void DroneJet ( )
{
	gedict_t	*te;
	vec3_t		vtemp;

	self->playerclass = self->playerclass + 1;
	self->s.v.nextthink = g_globalvars.time + 0.9;
	sound( self, 0, "weapons/drone.wav", 1, 1 );
	te=world;
	te = trap_find( te, FOFS( s.v.classname ), "player" );
	while( te )
	{
		if ( self != te )
		{
			if ( te->team_no != 0 && te->team_no != self->team_no )
			{
				if ( te->s.v.health )
				{
					if ( visible( te ) )
					{
						if ( te->is_undercover != 1 )
						{
							vtemp[0] =
								te->s.v.origin[0] - self->s.v.origin[0];
							vtemp[1] =
								te->s.v.origin[1] - self->s.v.origin[1];
							vtemp[2] =
								te->s.v.origin[2] - self->s.v.origin[2];
							DroneLaunchLaser ( self->s.v.origin, vtemp );
						}
					}
				}
			}
		}
		te = trap_find( te, FOFS( s.v.classname ), "player" );
	}
	te=world;
	te = trap_find( te, FOFS( s.v.classname ), "building_sentrygun" );
	while ( te )
	{
		if ( te->team_no != 0 && te->team_no != self->team_no )
		{
			if ( te->s.v.health )
			{
				if ( visible( te ) )
				{
					vtemp[0] =
						( te->s.v.origin[0] + 0 ) - self->s.v.origin[0];
					vtemp[1] =
						( te->s.v.origin[1] + 0 ) - self->s.v.origin[1];
					vtemp[2] =
						( te->s.v.origin[2] + 24 ) - self->s.v.origin[2];
					DroneLaunchLaser ( self->s.v.origin, vtemp );
				}
			}
		}
		te = trap_find( te, FOFS( s.v.classname ), "building_sentrygun" );
	}
	te=world;
	te = trap_find( te, FOFS( s.v.classname ), "bot" );
	while ( te )
	{
		if ( self != te )
		{
			if ( te->team_no != 0 && te->team_no != self->team_no )
			{
				if ( te->s.v.health )
				{
					if ( visible ( te ) )
					{
						vtemp[0] =
							te->s.v.origin[0] - self->s.v.origin[0];
						vtemp[1] =
							te->s.v.origin[1] - self->s.v.origin[1];
						vtemp[2] =
							te->s.v.origin[2] - self->s.v.origin[2];
						DroneLaunchLaser ( self->s.v.origin, vtemp );
					}
				}
			}
		}
		te = trap_find( te, FOFS( s.v.classname ), "bot" );
	}
	if ( self->playerclass > 10 )
		self->s.v.think = ( func_t ) DroneFall;
}

// Laser railgun
// in engineer.c