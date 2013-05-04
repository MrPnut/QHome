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
 *  $Id: mtf_medic.c,v 1.00 2006/09/15 XavioR Exp $
 */
#include "g_local.h"

// MTF Medic Functions
// Heal Self (thx plast for original code)
void HealSelf ( )
{
	float   healed;
	gedict_t *te;

	// Only medic's can heal themselves
	if ( self->playerclass != PC_MEDIC )
		return;

	// Costs 1 type 2 gren
	if ( self->no_grenades_2 < 1 ) {
		G_sprint( self, 2, "Not enough type 2 grenades... get more\n" );
		return;
	}

	healed = 0;

	for ( te=world ; (te = trap_find( te, FOFS( s.v.classname ), "timer" ));)
	{
		if ( te->s.v.owner != g_globalvars.self )
			continue;
		if ( te->s.v.think != ( func_t ) ConcussionGrenadeTimer &&
			te->s.v.think != ( func_t ) OldConcussionGrenadeTimer )
			continue;
		if ( tf_data.old_grens == 1 )
			stuffcmd( self, "v_idlescale 0\n" );
//			stuffcmd( self, "v_idlescale 0\nfov 90\n" );
		dremove( te );
		G_sprint ( self, 2, "you have been healed of your concussion\n" );
		healed = 1;
		break;
	}
	if ( self->tfstate & TFSTATE_HALLUCINATING )
	{
	    for ( te=world ; (te = trap_find( te, FOFS( s.v.classname ), "timer" ));)
		{
			if ( te->s.v.owner != g_globalvars.self )
				continue;
			if ( te->s.v.think != ( func_t ) HallucinationTimer )
				continue;

			self->tfstate -= ( self->tfstate & TFSTATE_HALLUCINATING );
			G_sprint (self, 2, "you have been healed of your hallucinations\n");
			healed = 1;
			
			ResetGasSkins(self);
			if ( tf_data.new_gas & GAS_MASK_PALETTE) 
				stuffcmd( self, "v_cshift; wait; bf\n" );
			dremove( te );
			break;
		}
		if ( !te )
			G_conprintf( "Warning: Error in Hallucination Timer logic.\n" );
	}
	if ( self->tfstate & TFSTATE_TRANQUILISED )
	{
	    for ( te=world ; (te = trap_find( te, FOFS( s.v.classname ), "timer" ));)
		{
			if ( te->s.v.owner != g_globalvars.self )
				continue;
			if ( te->s.v.think != ( func_t ) TranquiliserTimer )
				continue;

			self->tfstate -= ( self->tfstate & TFSTATE_TRANQUILISED );
			TeamFortress_SetSpeed( self );
			G_sprint ( self, 2, "you have been healed of your tranquilisation\n" );
			healed = 1;
			dremove( te );
			break;
		}
		if ( !te )
			G_conprintf( "Warning: Error in Tranquilisation Timer logic.\n" );
	}
	if ( self->FlashTime > 0 )
	{
	    for ( te=world ; (te = trap_find( te, FOFS( s.v.netname ), "flashtimer" ));)
		{
			if ( te->s.v.owner != g_globalvars.self )
				continue;
			if ( strneq( te->s.v.classname, "timer" ) )
				continue;

			self->FlashTime = 0;
			healed = 1;
			if ( tf_data.new_flash )
				disableupdates( self, -1 );	/* server-side flash */
			break;
		}
		if ( !te )
		{
			G_conprintf( "Warning: Error in Flash Timer logic.\n" );
			self->FlashTime = 0;
		}
	}
	// Leg shots
	if ( self->leg_damage > 0 )
	{
		G_sprint ( self, 2, "Your leg wound has been healed!\n" );
		self->leg_damage = 0;
		TeamFortress_SetSpeed ( self );
		healed = 1;
	}
	if ( self->numflames > 0 )
	{
		sound( self, 1, "items/r_item1.wav", 1, 1 );
		self->numflames = 0;
		G_sprint ( self, 2, "The flames have been doused!\n" );
		healed = 1;
		return;
	}

	if (healed == 0)
		G_sprint( self, 2, "You have nothing to cure.\n" );
	else {
		self->no_grenades_2 = self->no_grenades_2 - 1;

		trap_WriteByte( MSG_MULTICAST, 23 );
		trap_WriteByte( MSG_MULTICAST, 11 );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
		trap_WriteCoord( MSG_MULTICAST, self->s.v.origin[2] );
		trap_multicast( PASSVEC3( self->s.v.origin ), 1 );

		sound ( self, 3, "items/r_item2.wav", 1, 1 );
	}
}
