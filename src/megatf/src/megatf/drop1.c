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
 *  $Id: drop1.c,v 1.00 2006/09/15 20:12:21 XavioR Exp $
 */
#include "g_local.h"
void FlashGrenadeExplode2 ( );

// Only droppables are handled here. 
// Other drop1 functions are handled in the class files

/*****
		Misc Functions
						*****/
void MagTimer ( )
{
	gedict_t *te;
	//char t_left;

	te = PROG_TO_EDICT( self->s.v.owner );
	te->option5 = te->option5 - 0.5;
	//t_left = ( char ) te->option5;

	sound ( te, 0, "ambient/100hzhum.wav", 1, 1 );
	setorigin( te, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] );
	G_centerprint( te, "You are stuck on a mag mine!\n%d\n", te->option5 );
	//G_centerprint( te, "You are stuck on a mag mine!\n", t_left, "\n" );
	if ( te->option5 < 2 ) {
		te->option5 = 0;
		G_sprint ( PROG_TO_EDICT(te->s.v.enemy), 2, "Your Mag Mine died...\n" );
		PROG_TO_EDICT(te->s.v.enemy)->has_syringe = 1;
		dremove ( self );
		return;
	}
	self->s.v.nextthink = ( g_globalvars.time + 0.5 );
}

void MagMineActivate ( )
{
//	gedict_t *owner = self->s.v.owner;

	if ( other->option == 0 )
	{
		PROG_TO_EDICT( self->s.v.owner )->has_syringe = 1;
		newmis = spawn ( );
		newmis->s.v.classname = "timer";
		newmis->s.v.netname = " FlashTimer";
		newmis->team_no = PROG_TO_EDICT( self->s.v.owner )->team_no;
		newmis->s.v.owner = EDICT_TO_PROG( other );
		other->s.v.enemy = self->s.v.owner;
		newmis->s.v.think = ( func_t ) MagTimer;
		newmis->s.v.nextthink = g_globalvars.time + 1;
		setorigin ( newmis, other->s.v.origin[0], other->s.v.origin[1], other->s.v.origin[2] );
		//newmis->s.v.origin = other->s.v.origin;
		other->is_squating = 0;
	}
	other->option5 = floor ( other->s.v.armorvalue * 0.25 ) + 3;

	if ( other->option5 > 15 )
		other->option5 = 15;
}

/*****
	Remove Mine Functions
						*****/
void RemoveAlarmMine ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 1, "Your batteries died in an alarm.. :(\n" );
	dremove ( self );
	return;
}

void RemoveAIDSMine ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 1, "Your syringe is sterile...\n" );
	PROG_TO_EDICT( self->s.v.owner )->has_syringe = 0;
	dremove ( self );
	return;
}

void RemoveFlashMine ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 1, "Your Flash mine fizzled...\n" );
	PROG_TO_EDICT( self->s.v.owner )->has_syringe = 0;
	dremove ( self );
	return;
}

void RemoveSpikeTrap ()
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 1, "Your Spike Trap is gone...\n" );
	PROG_TO_EDICT( self->s.v.owner )->has_syringe = 0;
	dremove ( self );
	return;
}

void RemoveMagnetMine ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	G_sprint( owner, 1, "Your Mag Mine is gone...\n" );
	PROG_TO_EDICT( self->s.v.owner )->has_syringe = 0;
	dremove ( self );
	return;
}

/*****
	Mine Touch Functions
						*****/
void AlarmMineTouch ( )
{
	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;

	if ( other->team_no == PROG_TO_EDICT( self->s.v.owner )->team_no )
	{
//		if ((((other.velocity_x < 5) && (other.velocity_y < 5)) && (other.is_squating == 1)))
		if ( ((other->s.v.velocity[0] > 5) && (other->s.v.velocity[1] < 5)) && (other->is_squating == 1) ) {
			self->s.v.think = ( func_t ) RemoveAlarmMine;
			self->s.v.nextthink = g_globalvars.time + .2;
		}
		else
			return;
	}
	else {
		other->s.v.effects = ( int ) other->s.v.effects | EF_BRIGHTLIGHT;
		sound( self, CHAN_ITEM, "misc/enemy.wav", 1, ATTN_NORM );
		G_sprint( other, 1, "You have tripped an ALARM!\n" );
//		G_sprint( other, 1, "Your ALARM was tripped!\n" );	// not sure why mtf prints this 2x
		G_sprint( PROG_TO_EDICT( self->s.v.owner ), 1, "        Your ALARM was tripped!\n" );
		self->pausetime = ( g_globalvars.time + 5 );
	}
	self->s.v.nextthink = g_globalvars.time + 10;

	if ( ( int ) self->s.v.effects & 4 )
		other->s.v.effects = ( int ) other->s.v.effects - ( ( int ) other->s.v.effects & 8 );
}

void AIDSMineTouch ( )
{
	gedict_t *BioInfection;
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->team_no == PROG_TO_EDICT( self->s.v.owner )->team_no )
		return;

	if ( other->playerclass == PC_MEDIC ) {
		RemoveAIDSMine( );
		G_sprint (other, 2, "You removed an enemy syringe! :)\n");
		G_sprint (owner, 2, " Syringe removed by enemy medic!\n");
		return;
	}

	if ( streq( other->s.v.classname, "player" ) ) {
		//deathmsg = 14;
		if ( other->playerclass == PC_MEDIC )
			return;
		other->tfstate = other->tfstate | TFSTATE_INFECTED;
		BioInfection = spawn(  );
		BioInfection->s.v.classname = "timer";
		BioInfection->s.v.owner = EDICT_TO_PROG( other );
		BioInfection->s.v.nextthink = g_globalvars.time + 2;
		BioInfection->s.v.think = ( func_t ) BioInfection_Decay;
		BioInfection->s.v.enemy = EDICT_TO_PROG( owner );
		other->infection_team_no = self->team_no;
	}
	sound (self, 3, "player/mpain6.wav", 1, 1);
	G_sprint ( other, 2, "You're infected by a syringe! :(\n" );
	G_sprint ( owner, 2, "Your syringe has Infected an enemy!\n" );
	G_bprint ( 1, other->s.v.netname );
	G_bprint ( 1, " was infected by " );
	G_bprint ( 1, owner->s.v.netname );
	G_bprint ( 1, "'s syringe\n" );
	self->pausetime = ( g_globalvars.time + 5 );
	self->s.v.nextthink = ( g_globalvars.time + 10 );
	RemoveAIDSMine (  );
}

void FlashMineTouch ( )
{
	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->team_no == PROG_TO_EDICT( self->s.v.owner )->team_no )
		return;
	if ( streq( other->s.v.classname, "player" ) ) {
		FlashGrenadeExplode2 ( );
		G_sprint( PROG_TO_EDICT(self->s.v.owner), 1, "Your Flash mine was tripped!\n" );
		//self.pausetime = (time + 5);
	}
	//self.nextthink = (time+ 1);
	self->s.v.nextthink = g_globalvars.time + .1;
	dremove (self);
}

void SpikeTrapTouch ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->invincible_finished >= g_globalvars.time ) {
		if ( other->invincible_sound < g_globalvars.time ) {
			sound ( other, 3, "items/protect3.wav", 1, 1 );
			other->invincible_sound = ( g_globalvars.time + 2 );
		}
		return;
	}
	if ( other->team_no == PROG_TO_EDICT( self->s.v.owner )->team_no )
		return;
	if ( other->playerclass == PC_HVYWEAP ) {
		RemoveSpikeTrap ( );
		G_sprint ( other, 2, "You removed an enemy trap!\n" );
		G_sprint ( owner, 2, "Spike Trap removed by enemy!\n" );
		sound ( self, 3, "weapons/tink2.wav", 1, 1 );
		return;
	}
	sound ( self, 3, "shambler/smack.wav", 1, 1 );
	G_sprint ( other, 2, "You stepped on a Spike Trap!\n" );
	G_sprint ( owner, 2, "Your Trap worked!\n" );
	self->pausetime = ( g_globalvars.time + 5 );
	self->s.v.nextthink = ( g_globalvars.time + 5 );
	//deathmsg = 200;
	TF_T_Damage ( other, owner, owner, 1, 2, 0 );
	if ( other->s.v.health <= 0 )
		return;
	other->leg_damage = other->leg_damage + 1;
	TeamFortress_SetSpeed ( other );
}

void MagnetMineTouch ( )
{
	gedict_t *owner = PROG_TO_EDICT( self->s.v.owner );

	if ( self->pausetime > g_globalvars.time )
		return;
	if ( !streq( other->s.v.classname, "player" ) )
		return;
	if ( other->s.v.health <= 0 )
		return;
	if ( other->is_undercover == TRUE )
		return;
	if ( other->team_no == PROG_TO_EDICT( self->s.v.owner )->team_no )
		return;

	if ( streq( other->s.v.classname, "player" ) ) {
		MagMineActivate ( );
		G_sprint ( other, 2, "You stepped on a Magnetic Mine! :(\n" );
		G_sprint ( owner, 2, "Your Magnet Mine worked!\n" );
		self->pausetime = ( g_globalvars.time + 5 );
	}
	self->s.v.nextthink = g_globalvars.time + 5;
	dremove ( self );
}

/*****
	Mine Drop Function
						*****/
void ThrowMine( float type )
{
	gedict_t *item;		// the mine/spike trap/wtva

	item = spawn(  );
	item->s.v.owner = EDICT_TO_PROG( self );
	item->s.v.classname = "drop1";
	item->team_no = self->team_no;
	setorigin( item, self->s.v.origin[0], self->s.v.origin[1], (self->s.v.origin[2] - 10));

	if (type == 1)
		item->s.v.angles[0] = 270;
	if (type == 2) {
		item->s.v.angles[0] = 0;
		item->s.v.health = 5;
		item->th_die = RemoveAIDSMine;
	}
	if (type == 4) {
		item->s.v.angles[0] = 90;
		item->s.v.health = 5;
		item->th_die = RemoveSpikeTrap;
	}
	item->s.v.angles[1] = 45;
	item->s.v.flags = 256;
	item->s.v.solid = 2;
	item->s.v.movetype = 6;
	item->s.v.velocity[0] = 0;
	item->s.v.velocity[1] = 0;
	item->s.v.velocity[2] = 0;
	setsize( item, -1, -1, 0, 1, 1, 6 );

	if ( type == 4 ) {
		setmodel ( item, "progs/spikebal.mdl" );
	}
	else {
		if ( type == 5 ) {
			setmodel ( item, "progs/tgib1.mdl" );
			item->s.v.skin = 1;
		}
		else {
			setmodel ( item, "progs/syringe.mdl" );
		}
	}

	if ( type == 1 )
		item->s.v.touch = ( func_t ) AlarmMineTouch;

	if ( type == 2 )
		item->s.v.touch = ( func_t ) AIDSMineTouch;

	if ( type == 3 ) {
		item->s.v.health = 1;
		item->th_die = RemoveFlashMine;
		item->s.v.touch = ( func_t ) FlashMineTouch;
		setmodel ( item, "progs/s_bubble.spr" );
	}

	if ( type == 4 )
		item->s.v.touch = ( func_t ) SpikeTrapTouch;

	if ( type == 5 ) {
		item->s.v.touch = ( func_t ) MagnetMineTouch;
		setmodel ( item, "progs/tgib1.mdl" );
	}

	item->s.v.nextthink = g_globalvars.time + 180;
	if (type == 1)
		item->s.v.think = ( func_t ) RemoveAlarmMine;
	if (type == 2)
		item->s.v.think = ( func_t ) RemoveAIDSMine;
	if (type == 3)
		item->s.v.think = ( func_t ) RemoveFlashMine;
	if (type == 4)
		item->s.v.think = ( func_t ) RemoveSpikeTrap;
	if (type == 5)
		item->s.v.think = ( func_t ) RemoveMagnetMine;
}

/*****
	Mine Pre-Drop Functions
						*****/
void A_Mine ( )
{
	if (self->s.v.ammo_cells < 50) {
		G_sprint( self, 1, "Not enough cells...\n" );
		return;
	}
	self->s.v.ammo_cells = 0;
	ThrowMine (1);
	G_sprint( self, 1, "You set an alarm!\n" );
	sound( self, CHAN_VOICE, "effects/beep.wav", 1, ATTN_NORM );
}

void B_Mine ( )
{
	gedict_t *te;
	te=world;
	if (self->has_syringe != 1) {
		G_sprint( self, 1, "1 syringe at a time!\n" );
		return;
	}

	te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	while ( te )
	{
		if ( te->s.v.owner == EDICT_TO_PROG( self ) ) {
			G_sprint( self, 1, "1 syringe at a time!\n" );
			return;
		}
		te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	}

	ThrowMine ( 2 );
	G_sprint( self, 1, "You dropped a syringe!\n" );
	sound( self, CHAN_VOICE, "weapons/tink1.wav", 1, ATTN_NORM );
}

void C_Mine ( )
{
	gedict_t *te;
	te=world;
	if (self->has_syringe != 1) {
		G_sprint( self, 1, "1 flash mine at a time!\n" );
		return;
	}

	if (self->last_used >= g_globalvars.time) {
		G_sprint( self, 1, "not ready yet...\n" );
		return;
	}
	te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	while ( te )
	{
		if ( te->s.v.owner == EDICT_TO_PROG( self ) ) {
			G_sprint( self, 1, "1 flash mine at a time!\n" );
			return;
		}
		te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	}
	ThrowMine ( 3 );
	G_sprint( self, 1, "You set a Flash mine!\n" );
	sound( self, CHAN_VOICE, "weapons/bounce.wav", 1, ATTN_NORM );
}

// spike trap
void S_Mine ( )
{
	gedict_t *te;
	float spikecount;

	spikecount=0;
	te=world;
	te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	while ( te )
	{
		if ( te->s.v.owner == EDICT_TO_PROG( self ) ) {
			spikecount = spikecount + 1;
		}
		te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	}
	if (spikecount >= 2) {
		G_sprint( self, 1, "2 spike traps at a time!\n" );
		return;
	}
	ThrowMine ( 4 );
	G_sprint( self, 1, "You set a Spike Trap!\n" );
	sound( self, CHAN_VOICE, "doors/meduse.wav", 1, ATTN_NORM );
}

void M_Mine ( )
{
	gedict_t *te;
	te=world;

	if (self->has_syringe != 1) {
		G_sprint( self, 1, "You get 1 Mag Mine at a time!\n" );
		return;
	}

	te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	while ( te )
	{
		if ( te->s.v.owner == EDICT_TO_PROG( self ) ) {
			G_sprint( self, 1, "You get 1 Mag Mine at a time!\n" );
			return;
		}
		te = trap_find( te, FOFS( s.v.classname ), "drop1" );
	}

	ThrowMine ( 5 );
	G_sprint( self, 1, "You set a Mag Mine!\n" );
	sound( self, CHAN_VOICE, "weapons/bounce.wav", 1, ATTN_NORM );
}
