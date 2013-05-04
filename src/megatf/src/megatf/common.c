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
 *  $Id: common.c,v 1.00 2006/09/15 XavioR Exp $
 */

/*	=========
	Common MTF Functions
				========  */
#include "g_local.h"

///////////////////////////////////////
///	Night Vision
void TeamFortress_NightVision (  )
{
	gedict_t *te, *tl;
	te=world;

	sound( self, 0, "items/nightvis.wav", 0.5, 1 );
	// find enemies and put lightning effects around them
	te = trap_find( te, FOFS( s.v.classname ), "player" );
	while( te )
	{
		if ( self != te )
		{
			if ( te->team_no != 0 )
			{
				if ( te->s.v.health > 1 )
				{
					if ( visible( te ) )
					{
						g_globalvars.msg_entity = EDICT_TO_PROG( self );
						tl = spawn(  );
						tl->s.v.owner = EDICT_TO_PROG( self );
						VectorCopy( te->s.v.origin, tl->s.v.origin );
						tl->s.v.origin[2] = tl->s.v.origin[2] + 32;
						trap_WriteByte( MSG_ONE, 23 );
						trap_WriteByte( MSG_ONE, 9 );
						WriteEntity( MSG_ONE, tl );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[2] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[2] + 5 );
						dremove( tl );
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
		if ( self != te )
		{
			if ( te->team_no != 0 )
			{
				if ( te->s.v.health > 1 )
				{
					if ( visible( te ) )
					{
						g_globalvars.msg_entity = EDICT_TO_PROG( self );
						tl = spawn(  );
						tl->s.v.owner = EDICT_TO_PROG( self );
						VectorCopy( te->s.v.origin, tl->s.v.origin );
						tl->s.v.origin[2] = tl->s.v.origin[2] + 32;
						trap_WriteByte( MSG_ONE, 23 );
						trap_WriteByte( MSG_ONE, 9 );
						WriteEntity( MSG_ONE, tl );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[2] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[2] + 5 );
						dremove( tl );
					}
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
			if ( te->team_no != 0 )
			{
				if ( te->s.v.health > 1 )
				{
					if ( visible( te ) )
					{
						g_globalvars.msg_entity = EDICT_TO_PROG( self );
						tl = spawn(  );
						tl->s.v.owner = EDICT_TO_PROG( self );
						VectorCopy( te->s.v.origin, tl->s.v.origin );
						tl->s.v.origin[2] = tl->s.v.origin[2] + 32;
						trap_WriteByte( MSG_ONE, 23 );
						trap_WriteByte( MSG_ONE, 9 );
						WriteEntity( MSG_ONE, tl );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, tl->s.v.origin[2] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[0] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[1] );
						trap_WriteCoord( MSG_ONE, te->s.v.origin[2] + 5 );
						dremove( tl );
					}
				}
			}
		}
		te = trap_find( te, FOFS( s.v.classname ), "bot" );
	}
}

///////////////////////////////////////
/// lay mode sizes
void lay ( )
{
	setsize( self, -16, -16, -24, 16, 16, 8 );
}

void unlay ( )
{
	setsize( self, -16, -16, -24, 16, 16, 32 );
}

////////////////////////////////////////
///		 Hologram
void HoloHumm ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	self->playerclass = self->playerclass + 1;
	self->s.v.nextthink = g_globalvars.time + 1;
	sound( self, 0, "ambient/100hzhum.wav", 1, 1 );
	if ( self->playerclass > 4 ) {
		trap_WriteByte( MSG_MULTICAST, SVC_TEMPENTITY );
		trap_WriteByte( MSG_MULTICAST, 11 );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
		trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
		if ( self->playerclass > 4 ) {
			owner->has_holo = 1;
			self->s.v.think = ( func_t ) SUB_Remove;
		}
	}
}

// spawn the actual holo
void HoloDood ( )
{
	gedict_t *holo;
	vec3_t vtemp;

	sound( self, 1, "weapons/railgr1a.wav", 0.6, 1 );
	holo = spawn(  );
	holo->s.v.owner = EDICT_TO_PROG( self );
	vtemp[0] =
		self->s.v.origin[0];
	vtemp[1] =
		self->s.v.origin[1];
	vtemp[2] =
		self->s.v.origin[2] + 24;
	setorigin( holo, PASSVEC3( vtemp ) );
	holo->s.v.angles[1] = self->s.v.angles[1];
	holo->s.v.angles[0] = self->s.v.angles[0];
	holo->s.v.skin = self->s.v.skin;
	holo->s.v.frame = self->s.v.frame;
	holo->s.v.colormap = self->s.v.colormap;
	holo->s.v.flags = 256;
	holo->s.v.solid = SOLID_TRIGGER;
	holo->s.v.effects = EF_DIMLIGHT;
	holo->s.v.movetype = MOVETYPE_TOSS;
	setmodel( holo, "progs/player.mdl" );
	setsize( holo, -16, -16, -24, 16, 16, 32 );
	holo->s.v.classname = "holo";
	holo->playerclass = 0;
	holo->s.v.nextthink = g_globalvars.time + 1;
	holo->s.v.think = ( func_t ) HoloHumm;
	trap_WriteByte( MSG_MULTICAST, SVC_TEMPENTITY );
	trap_WriteByte( MSG_MULTICAST, 11 );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
	trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
}

// pre-holo spawn
void MakeHolo ( )
{
	if ( self->playerclass == 0 )
		return;
	if ( self->has_holo != 1 )
	{
		G_sprint( self, 2, "holograph already running!\n" );
		return;
	}
	self->has_holo = 0;
	HoloDood ();
	G_sprint (self, 2, "Holographic replica active!\n" );
}

// Shell Eject
void ShellHit ( )
{
	if ( self->s.v.skin == 1 )
		sound( self, 3, "weapons/shell2.wav", 0.5, 1 );
	else {
		if ( g_random(  ) < 0.5 )
			sound( self, 3, "weapons/tink1.wav", 0.5, 1 );
		else
			sound( self, 3, "weapons/tink2.wav", 0.6, 1 );
	}
	return;
}

void EjectShell ( )
{
	vec3_t	s_org;

	newmis = spawn (  );
	newmis->s.v.movetype = MOVETYPE_BOUNCE;
	newmis->s.v.solid = SOLID_TRIGGER;
	newmis->team_no = self->team_no;
	makevectors( self->s.v.v_angle );

	if ( self->s.v.v_angle[0] )	{
		if ( self->current_weapon == WEAP_SNIPER_RIFLE )
		{
			newmis->s.v.velocity[0] = g_globalvars.v_forward[0] * 70 + g_globalvars.v_up[0] * 90 + ( ( g_random( ) * g_globalvars.v_right[0] ) * -150 );
			newmis->s.v.velocity[1] = g_globalvars.v_forward[1] * 70 + g_globalvars.v_up[1] * 90 + ( ( g_random( ) * g_globalvars.v_right[1] ) * -150 );
			newmis->s.v.velocity[2] = g_globalvars.v_forward[2] * 70 + g_globalvars.v_up[2] * 90 + ( ( g_random( ) * g_globalvars.v_right[2] ) * -150 );
		}
		else
		{
			newmis->s.v.velocity[0] = g_globalvars.v_forward[0] * 70 + g_globalvars.v_up[0] * 90 + ( ( g_random( ) * g_globalvars.v_right[0] ) * 150 );
			newmis->s.v.velocity[1] = g_globalvars.v_forward[1] * 70 + g_globalvars.v_up[1] * 90 + ( ( g_random( ) * g_globalvars.v_right[1] ) * 150 );
			newmis->s.v.velocity[2] = g_globalvars.v_forward[2] * 70 + g_globalvars.v_up[2] * 90 + ( ( g_random( ) * g_globalvars.v_right[2] ) * 150 );
		}
	}
	else {
		aim( newmis->s.v.velocity );
		if ( self->current_weapon == WEAP_SNIPER_RIFLE )
		{
			newmis->s.v.velocity[0] = g_globalvars.v_forward[0] * 70 + g_globalvars.v_up[0] * 90 + ( ( g_random( ) * g_globalvars.v_right[0] ) * -150 );
			newmis->s.v.velocity[1] = g_globalvars.v_forward[1] * 70 + g_globalvars.v_up[1] * 90 + ( ( g_random( ) * g_globalvars.v_right[1] ) * -150 );
			newmis->s.v.velocity[2] = g_globalvars.v_forward[2] * 70 + g_globalvars.v_up[2] * 90 + ( ( g_random( ) * g_globalvars.v_right[2] ) * -150 );
		}
		else
		{
			newmis->s.v.velocity[0] = g_globalvars.v_forward[0] * 70 + g_globalvars.v_up[0] * 90 + ( ( g_random( ) * g_globalvars.v_right[0] ) * 150 );
			newmis->s.v.velocity[1] = g_globalvars.v_forward[1] * 70 + g_globalvars.v_up[1] * 90 + ( ( g_random( ) * g_globalvars.v_right[1] ) * 150 );
			newmis->s.v.velocity[2] = g_globalvars.v_forward[2] * 70 + g_globalvars.v_up[2] * 90 + ( ( g_random( ) * g_globalvars.v_right[2] ) * 150 );
		}
		newmis->s.v.velocity[2] = g_random(  ) * 90;
	}
	if ( g_random(  ) < 0.3 )
		SetVector( newmis->s.v.avelocity, 2000, 3000, 4000 );
	else
	{
		if ( g_random(  ) < 0.6 )
			SetVector( newmis->s.v.avelocity, 4000, 2000, 3000 );
		else
			SetVector( newmis->s.v.avelocity, 7000, 5000, 1000 );
	}

	s_org[0] = self->s.v.origin[0] + ( g_globalvars.v_forward[0] * 10 ) + 0;
	s_org[1] = self->s.v.origin[1] + ( g_globalvars.v_forward[1] * 10 ) + 0;
	s_org[2] = self->s.v.origin[2] + ( g_globalvars.v_forward[2] * 10 ) + 20;
	setsize( newmis, 0, 0, 0, 0, 0, 0 );
	//setorigin( newmis, ( ( self->s.v.origin + ( v_forward * 10 ) ) + '0 0 20' ) );
	setorigin( newmis, PASSVEC3( s_org ) );

	if ( self->playerclass == PC_HVYWEAP ) {
		newmis->s.v.nextthink = g_globalvars.time + 1.5;
		setmodel( newmis, "progs/20mmcase.mdl" );
	}
	else {
		newmis->s.v.nextthink = g_globalvars.time + 3;
		setmodel( newmis, "progs/shell2.mdl" );
	}

	newmis->s.v.think = ( func_t ) SUB_Remove;
	newmis->s.v.touch = ( func_t ) ShellHit;
	if ( self->current_weapon == WEAP_SHOTGUN || self->current_weapon == WEAP_SUPER_SHOTGUN )
		newmis->s.v.skin = 0;
	else
		newmis->s.v.skin = 1;
}

// Death message wrapper
void mtf_deathmsg ( float dmsg )
{
	tf_data.megatf_dmg = (int) dmsg;	// tells obituary to display a megatf death message
	tf_data.deathmsg = (int) dmsg;
}

// explosion stuff
// explosion sequence (used by proxi explosion funct)
void    s2_explode1(  );
void    s2_explode2(  );
void    s2_explode3(  );
void    s2_explode4(  );
void    s2_explode5(  );
void    s2_explode6(  );
void    s2_explode7(  );
void    s2_explode8(  );
void	check_water(  );
void s2_explode1(  )
{
	self->s.v.frame = 0;
	self->s.v.think = ( func_t ) s2_explode2;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode2(  )
{
	self->s.v.frame = 1;
	self->s.v.think = ( func_t ) s2_explode3;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode3(  )
{
	self->s.v.frame = 2;
	self->s.v.think = ( func_t ) s2_explode4;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode4(  )
{
	self->s.v.frame = 3;
	self->s.v.think = ( func_t ) s2_explode5;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode5(  )
{
	self->s.v.frame = 4;
	self->s.v.think = ( func_t ) s2_explode6;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode6(  )
{
	self->s.v.frame = 5;
	self->s.v.think = ( func_t ) s2_explode7;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode7(  )
{
	self->s.v.frame = 5;
	self->s.v.think = ( func_t ) s2_explode8;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void s2_explode8(  )
{
	self->s.v.frame = 5;
	self->s.v.think = ( func_t ) SUB_Remove;
	self->s.v.nextthink = g_globalvars.time + 0.1;
	check_water();
}

void make_explosion ( )
{

	newmis = spawn ( );
	setmodel( newmis, "progs/s_expl.spr" );
	setorigin( newmis, PASSVEC3( self->s.v.origin ) );
	newmis->s.v.movetype = 8;
	newmis->s.v.solid = SOLID_NOT;
	sound ( newmis, 0, "weapons/flmfire2.wav", 1, 1 );
	newmis->s.v.effects = 8;
	newmis->s.v.velocity[2] = ( g_random () * 250.000000 );
	newmis->s.v.nextthink = g_globalvars.time + 0.05;
	newmis->s.v.think = ( func_t ) s2_explode1;
	newmis->s.v.nextthink = g_globalvars.time + 0.05;
}


