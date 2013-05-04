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
 *  $Id: mtf_soldier.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"
void make_explosion ( );
void MushroomFlame ( );

// MegaTF soldier functions

// Proximity Mine
void Prox_Disarm ( )
{
	if ( !streq( other->s.v.classname, "player" ) )
		return;

	if ( ( other->playerclass == PC_SCOUT ) && ( other->team_no != self->team_no ) )
	{
		sound( other, 2, "items/flare1.wav", 1, 1 );
		other->real_frags = other->real_frags + 1;
		dremove( self );
	}
	return;
}

void ProxiDie ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	self->s.v.effects = 4;
	make_explosion(  );
	self->s.v.takedamage = 0;
	mtf_deathmsg( 45 );
	T_RadiusDamage ( self, owner, 200, world );
	G_sprint ( owner, 2, "Your Proxi Mine is gone...\n" );
	dremove ( self );
}

void FoundBastard ( )
{
	float leng;
	vec3_t  vtemp;
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	self->s.v.touch = ( func_t ) Prox_Disarm;
	sound ( self, 3, "effects/beep.wav", 1, 1 );
	self->s.v.skin = owner->team_no + 3;
	VectorSubtract( PROG_TO_EDICT( self->s.v.enemy )->s.v.origin, self->s.v.origin, vtemp );
	leng = vlen( vtemp );

	if ( leng <= ( float ) self->height ) {
		self->height = leng;
		self->s.v.nextthink = g_globalvars.time + ( ( leng / 1000 ) * 1.5 );
		self->s.v.think = ( func_t ) FoundBastard;
	}
	else {
		self->s.v.effects = 4;
		make_explosion (  );
		MushroomFlame (  );
		trap_WriteByte ( MSG_MULTICAST, 23 );
		trap_WriteByte ( MSG_MULTICAST, 3 );
		trap_WriteCoord ( MSG_MULTICAST, self->s.v.origin[0] );
		trap_WriteCoord ( MSG_MULTICAST, self->s.v.origin[1] );
		trap_WriteCoord ( MSG_MULTICAST, self->s.v.origin[2] );
		trap_multicast( PASSVEC3( self->s.v.origin ), 1 );
		mtf_deathmsg( 45 );
		self->s.v.takedamage = 0;
		T_RadiusDamage ( self, PROG_TO_EDICT( self->s.v.owner ), 200, world );
		dremove ( self );
	}
}

void ProxArmed ( )
{
	gedict_t *te;
	vec3_t vec_a;
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	self->s.v.velocity[0] = 0;self->s.v.velocity[1] = 0;self->s.v.velocity[2] = 0;
	self->s.v.nextthink = g_globalvars.time + 1;
	self->s.v.think = ( func_t ) ProxArmed;
	sound ( self, 3, "effects/beep.wav", 0.3, 1 );
	self->playerclass = self->playerclass + 1;

	if ( self->s.v.skin == owner->team_no )
		self->s.v.skin = 0;
	else
		self->s.v.skin = owner->team_no;

	te = trap_find( world, FOFS( s.v.classname ), "player" );
	while ( te )
	{
		if ( owner != te )
		{
			if ( ( te->team_no != 0 ) && ( te->team_no != self->team_no ) )
			{
				if ( te->s.v.health > 1 )
				{
					if ( visible( te ) )
					{
						if ( te->undercover_team != self->team_no )
						{
							VectorSubtract( te->s.v.origin, self->s.v.origin, vec_a )
							//vec_a = te->s.v.origin - self->s.v.origin;
							if ( vlen( vec_a ) < 200 )
							{
								self->height = vlen ( vec_a );
								self->s.v.enemy = EDICT_TO_PROG( te );
								self->s.v.nextthink = 0.2;
								self->s.v.think = ( func_t ) FoundBastard;
							}
						}
					}
				}
			}
		}
		te = trap_find( te, FOFS( s.v.classname ), "player" );
	}

	if ( self->playerclass > 300 || owner->playerclass != PC_SOLDIER )
		dremove ( self );
}

void ProxAttach ( )
{
	self->s.v.touch = ( func_t ) SUB_Null;
	self->s.v.think = ( func_t ) ProxArmed;
	self->s.v.nextthink = ( g_globalvars.time + 0.5 );
	sound ( newmis, 0, "buttons/switch02.wav", 1, 1 );
}

void OneProx ( )
{
		G_sprint ( self, 2, "1 proximity grenade at a time!\n" );
		if ( streq( self->s.v.weaponmodel, "progs/proxgren.mdl" ) ) {
			self->current_weapon = W_BestWeapon(  );
			self->s.v.weaponmodel = "progs/v_rock2.mdl";
			self->s.v.currentammo = self->s.v.ammo_rockets;
			Attack_Finished ( 0.7 );
			self->s.v.impulse = 0;
			self->StatusRefreshTime = g_globalvars.time + 0.1;
		}
}

void Prox ( )
{
	vec3_t vec_a, vec_b, vec_c, source, dest;
	gedict_t *te;

	vec_c[0] = 0;vec_c[1] = 0;vec_c[2] = 16;

	te = trap_find( world, FOFS( s.v.classname ), "bot" );
	while ( te )
	{
		if ( PROG_TO_EDICT( te->s.v.owner ) == self )
		{
/*			makevectors( self->s.v.v_angle );
			VectorAdd( vec_c, self->s.v.origin, source );
			//source = ( self->s.v.origin + '0 0 16' );
			traceline ( source, ( source + ( v_forward * 64 ) ), 0, world );*/
			makevectors( self->s.v.v_angle );
			VectorCopy( self->s.v.origin, source );
			source[2] += 16;
			VectorScale( g_globalvars.v_forward, 64, dest );
			VectorAdd( dest, source, dest ) ;
			traceline( PASSVEC3( source ), PASSVEC3( dest ), false, world );

//			if ( g_globalvars.trace_fraction == 1.0 ) {
			OneProx(  );
			return;
//			}

/*			// disarm self-proxy - this is a megatf United feature
			if ( trace_ent.classname == "bot" && trace_ent.owner == self )
			{
				G_sprint( self, 2, "You disarm your proxy grenade\n" );
				self->no_grenades_2 = self->no_grenades_2 + 2;
				if ( self->no_grenades_2 > 4 )
					self->no_grenades_2 = 4;
				dremove( trace_ent );
				return;
			}
			else
			{
				OneProx(  );
				return;
			}*/
		}
		te = trap_find( world, FOFS( s.v.classname ), "bot" );
	}
	if ( self->no_grenades_2 < 2 )
	{
		G_sprint ( self, 2, "Not enough type 2 grenades... get more\n" );
		if ( streq( self->s.v.weaponmodel, "progs/proxgren.mdl" ) ) {
			self->s.v.weaponmodel = "progs/v_rock2.mdl";
			self->s.v.currentammo = self->s.v.ammo_rockets;
			Attack_Finished ( 0.7 );
			self->s.v.impulse = 0;
			self->StatusRefreshTime = ( g_globalvars.time + 0.1 );
//			self->tfstate = ( self->tfstate - ( self->tfstate & 2 ) );
			self->tfstate = self->tfstate - TFSTATE_RELOADING;
		}
		return;
	}
	if ( !streq( self->s.v.weaponmodel, "progs/proxgren.mdl" ) ) {
		self->tfstate = self->tfstate | TFSTATE_RELOADING;
		self->oldweaponmodel = self->s.v.weaponmodel;
		self->oldcurrentammo = self->s.v.currentammo;
		self->s.v.currentammo = 1;
		self->s.v.weaponmodel = "progs/proxgren.mdl";
		self->s.v.weaponframe = 0;
		return;
	}
	makevectors( self->s.v.v_angle );
	VectorScale( g_globalvars.v_forward, 10, vec_a );
	VectorAdd( vec_a, self->s.v.origin, vec_a );		// parm 1 vec_a?
	vec_a[2] = self->s.v.absmin[2] + self->s.v.size[2] * 0.7;
	VectorScale( g_globalvars.v_forward, 70, vec_b );
	VectorAdd( vec_b, vec_a, vec_b );
	traceline( PASSVEC3( vec_a ), PASSVEC3( vec_b ), 0, self );
	//trace_ent = PROG_TO_EDICT( g_globalvars.trace_ent );
	VectorScale( g_globalvars.v_forward, 8, vec_c );
	VectorSubtract( g_globalvars.trace_endpos, vec_c, vec_b );
//	vec_b = trace_endpos - ( v_forward * 8 );
	if ( g_globalvars.trace_fraction != 1 )
	{
		sound ( self, 3, "buttons/switch02.wav", 1, 1 );
		G_sprint ( self, 2, "Proximity mine set!\n" );
		self->no_grenades_2 = self->no_grenades_2 - 2;
		newmis = spawn (  );
		newmis->s.v.owner = EDICT_TO_PROG( self );
		newmis->s.v.weapon = 41;
		newmis->s.v.movetype = 0;
		newmis->s.v.solid = 2;
		newmis->s.v.classname = "bot";
		newmis->s.v.netname = "proxi";
		newmis->team_no = self->team_no;
		newmis->s.v.health = 3;
		newmis->s.v.takedamage = 2;
		newmis->th_die = ProxiDie;
		setsize ( newmis, -3, -3, -3, 3, 3, 3 );
		newmis->s.v.angles[0] = 0;newmis->s.v.angles[1] = 0;newmis->s.v.angles[2] = 0;
		newmis->s.v.nextthink = g_globalvars.time + 1;
		newmis->playerclass = 0;
		newmis->s.v.think = ( func_t ) ProxAttach;
		newmis->s.v.skin = 0;
		newmis->s.v.frame = 1;
		setmodel ( newmis, "progs/proxgren.mdl" );
		setorigin ( newmis, PASSVEC3( vec_b ) );
		if ( self->oldweaponmodel != "" )
		{
			self->tfstate = ( self->tfstate - ( self->tfstate & TFSTATE_RELOADING ) );
		}
		self->s.v.weaponmodel = self->oldweaponmodel;
		self->s.v.currentammo = self->oldcurrentammo;
		Attack_Finished ( 0.7 );
		self->s.v.impulse = 0;
		self->StatusRefreshTime = g_globalvars.time + 0.1;
	}
}