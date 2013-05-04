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
 *  $Id: obituary.c,v 1.00 2006/10/06 XavioR Exp $
 */
#include "g_local.h"

// MegaTF client obituary
// I tried to wrap this as best I could, but it's still ugly.
void Mega_ClientObituary( gedict_t* targ, gedict_t* attacker ) 
{
	float rnum, deathmsg;
	char   *deathstring = "";
	char   *deathstring2 = "";

	deathmsg = ( float ) tf_data.deathmsg;
#ifdef fte_rankings
	if (FTE_Server == 1)
		Rank_AddDeaths(targ, 1);
#endif
// 'fer code caller!
	/*
	if (deathmsg > 0 && deathmsg < 199)
	{
		if (attacker)
		{
			attacker->num_kills = attacker->num_kills + 1;
                  if ((targ->off_hook > g_globalvars.time))
                  {
                  attacker->num_kills = attacker->num_kills - 1;
                  targ->off_hook = 0;
                  }
			if (attacker->fertracker != 1)
			{
				attacker->fertracker = 1;
				fertimer (attacker);
			}
			if (attacker == targ)
			{
				attacker->num_kills = attacker->num_kills - 1;
			}*/
#ifdef fte_rankings
			if (FTE_Server == 1 && attacker != targ)
				Rank_AddFrags(attacker, 1);
#endif
			/*
			if (targ->off_hook != 1)
			{
//So disps, Drones, and heads dont count as fers
				if (streq(targ->s.v.classname, "building_dispenser")) {
				attacker->num_kills = attacker->num_kills - 1; }
				if (targ->s.v.think (func_t) == DroneJet) {
				attacker->num_kills = attacker->num_kills - 1; }
				if (targ->s.v.touch (func_t) == KickHead) {
				attacker->num_kills = attacker->num_kills - 1; }
			}
		}
	}*/
// end 'fer caller
	rnum = g_random();
	if (streq(targ->s.v.classname, "player"))
	{
		if (streq(attacker->s.v.classname, "teledeath"))
		{
			G_bprint(1, targ->s.v.netname);
			G_bprint(1, " got in the way of ");
			G_bprint(1, PROG_TO_EDICT(attacker->s.v.owner)->s.v.netname);
			G_bprint(1, "\n");
			if (PROG_TO_EDICT(attacker->s.v.owner)->team_no != targ->team_no || PROG_TO_EDICT(attacker->s.v.owner)->team_no < 1)
			{
				PROG_TO_EDICT(attacker->s.v.owner)->real_frags = PROG_TO_EDICT(attacker->s.v.owner)->real_frags + 1;
			}
			if (!(tf_data.toggleflags & 128))
			{
				PROG_TO_EDICT(attacker->s.v.owner)->s.v.frags = PROG_TO_EDICT(attacker->s.v.owner)->real_frags;
			}
			return;
		}
		if (streq(attacker->s.v.classname, "teledeath2"))
		{
			G_bprint(1, "Satan's power deflects ");
			G_bprint(1, targ->s.v.netname);
			G_bprint(1, "'s telefrag\n");
			targ->real_frags = targ->real_frags - 1;
			if (!(tf_data.toggleflags & 128))
			{
				targ->s.v.frags = targ->real_frags;
			}
			logfrag(targ, targ);
			return;
		}
		if (streq(attacker->s.v.classname, "info_tfgoal"))
		{
			if (attacker->deathtype != "")
			{
				G_bprint(1, targ->s.v.netname);
				G_bprint(1, attacker->deathtype);
			}
			logfrag(targ, targ);
			return;
		}
		if (streq(attacker->s.v.classname, "player") || attacker->s.v.classname  == "bot")
		{
			if (targ == attacker)
			{
				attacker->real_frags = attacker->real_frags - 1;
				if (!(tf_data.toggleflags & 128))
				{
					attacker->s.v.frags = attacker->real_frags;
				}
				G_bprint(1, targ->s.v.netname);
				if (deathmsg == 8)
				{
					deathstring = " grenades himself\n";
				}
				else
				{
					if (deathmsg == 43)
					{
						deathstring = " has himself bombed\n";
					}
					else
					{
						if (deathmsg == 45)
						{
							deathstring = " hugs his proximity grenade\n";
						}
						else
						{
							if (deathmsg == 9)
							{
								deathstring = " nails himself\n";
							}
							else
							{
								if (deathmsg == 10)
								{
									deathstring = " is blown to bits\n";
								}
								else
								{
									if (deathmsg == 11)
									{
										deathstring = " pipebombs himself...\nWhat a dumb-ass!\n";
									}
									else
									{
										if (deathmsg == 24)
										{
											deathstring = " gags on his own gas... pew!\n";
										}
										else
										{
											if (deathmsg == 30)
											{
												deathstring = " nukes his own ammo\n";
											}
											else
											{
												if (deathmsg == 35)
												{
													deathstring = " flash grenade himself to death\n";
												}
												else
												{
													if (deathmsg == 31)
													{
														deathstring = " detonates an ammo box too close to him\n";
													}
													else
													{
														if (deathmsg == 12)
														{
															deathstring = " detpacks himself\n That was smart!\n";
														}
														else
														{
															if (deathmsg == 13)
															{
																deathstring = " died impossibly!\n";
															}
															else
															{
																if (deathmsg == 6)
																{
																	if (rnum <= 0.5)
																	{
																		deathstring = " checks if his weapon is loaded\n";
																	}
																	else
																	{
																		deathstring = " hates himself\n";
																	}
																}
																else
																{
																	if (deathmsg == 41)
																	{
																		deathstring = "'s Laser Drone malfunctioned\n";
																	}
																	else
																	{
																		if (deathmsg == 33)
																		{
																			deathstring = " chars himself with an incendiary rocket\n";
																		}
																		else
																		{
																			if (deathmsg == 5)
																			{
																				deathstring = " tries to put the pin back in\n";
																			}
																			else
																			{
																				if (deathmsg == 15)
																				{
																					if (attacker->playerclass == 1)
																					{
																						deathstring = "'s JetPack malfunctions\n";
																					}
																					else
																					{
																						deathstring = " bakes himself\n";
																					}
																				}
																				else
																				{
																					if (deathmsg == 7 && targ->s.v.waterlevel > 0)
																					{
																						G_bprint(1, " shocks himself to death.\n");
																						return;
																					}
																					else
																					{
																						if (deathmsg == 40)																					
																						{
																							deathstring = " couldn't outrun his airspam\n";																							
																						}
																						else
																						{
																							if (deathmsg == 201)
																							{
																								deathstring = " used his dispenser for all the wrong reasons\n";																								
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				G_bprint(1, deathstring);
				return;
			}
			else
			{
				if (teamplay && attacker->team_no == targ->team_no && attacker->team_no > 0)
				{
					if (attacker->team_no > 0 && attacker->team_no == targ->team_no)
					{
						attacker->real_frags = attacker->real_frags - 1;
					}
					else
					{
						attacker->real_frags = attacker->real_frags + 1;
					}
					if (!(tf_data.toggleflags & 128))
					{
						attacker->s.v.frags = attacker->real_frags;
					}
					if (rnum < 0.25)
					{
						deathstring = " mows down a teammate\n";
					}
					else
					{
						if (rnum < 0.5)
						{
							deathstring = " checks his glasses\n";
						}
						else
						{
							if (rnum < 0.75)
							{
								deathstring = " gets a frag for the other team\n";
							}
							else
							{
								deathstring = " loses another friend\n";
							}
						}
					}
					if (deathmsg == 23)
					{
						G_bprint(1, targ->s.v.netname);
						G_bprint(1, " didn't survive the operation.\n");
						return;
					}
					G_bprint(1, attacker->s.v.netname);
					G_bprint(1, deathstring);
					return;
				}
				else
				{
					attacker->real_frags = attacker->real_frags + 1;
					logfrag(attacker, targ);
					if (!(tf_data.toggleflags & 128))
					{
						attacker->s.v.frags = attacker->real_frags;
					}
					if (deathmsg == 43)
					{
						deathstring = " was bombed by ";
						deathstring2 = "'s AirStrike call\n";
					}
					if (deathmsg == 45)
					{
						deathstring = " gets too friendly with ";
						deathstring2 = "'s Proxi grenade\n";
					}
					if (deathmsg == 46)
					{
						deathstring = " gets shredded by ";
						deathstring2 = "'s 20mm cannon\n";
					}
					if (deathmsg == 8)
					{
						deathstring = " tries to hatch ";
						deathstring2 = "'s grenade\n";
					}
					else
					{
						if (deathmsg == 9)
						{
							deathstring = " gets perforated by ";
							deathstring2 = "'s nail grenade\n";
						}
						else
						{
							if (deathmsg == 10)
							{
								deathstring = " gets juiced by ";
								deathstring2 = "'s Mirv grenade\n";
							}
							else
							{
								if (deathmsg == 40)
								{
									deathstring = " is shreaded by ";
									deathstring2 = "'s AirMirv\n";
								}
								else
								{
									if (deathmsg == 11)
									{
										deathstring = " is shreaded by ";
										deathstring2 = "'s pipebomb trap\n";
									}
									else
									{
										if (deathmsg == 24)
										{
											deathstring = " sniffs to much of ";
											deathstring2 = "'s glue\n";
										}
										else
										{
											if (deathmsg == 30)
											{
												deathstring = "'s gets vaporized by ";
												deathstring2 = "'s EMP grenade.\n";
											}
											else
											{
												if (deathmsg == 35)
												{
													deathstring = "'s brain is fried by ";
													deathstring2 = "'s flash grenade\n";
												}
												else
												{
													if (deathmsg == 31)
													{
														deathstring = " stands near some ammo as ";
														deathstring2 = "'s EMP nukes it\n";
													}
													else
													{
														if (deathmsg == 12)
														{
															deathstring = " is nuked by ";
															deathstring2 = "'s detpack\n";
														}
														else
														{
															if (deathmsg == 16)
															{
																deathstring = " cut the red wire of ";
																deathstring2 = "'s detpack\n";
															}
															else
															{
																if (deathmsg == 13)
																{
																	deathstring = " dies from ";
																	deathstring2 = "'s social disease\n";
																	sound(targ, 0, "player/mpain6.wav", 1, 1);
																}
																else
																{
																	if (deathmsg == 14)
																	{
																		deathstring = " escapes infection from ";
																		deathstring2 = " by dying first\n";
																	}
																	else
																	{
																		if (deathmsg == 5)
																		{
																			deathstring = " swallows ";
																			deathstring2 = "'s grenade\n";
																			if (targ->s.v.health < -40)
																			{
																				deathstring = " was split in half by ";
																				deathstring2 = "'s grenade\n";
																			}
																		}
																		else
																		{
																			if (deathmsg == 6)
																			{
																				deathstring = " is reamed by ";
																				deathstring2 = "'s rocket\n";
																				if (targ->s.v.health < -40)
																				{
																					deathstring = "'s bunghole was ripped by ";
																					deathstring2 = "'s rocket\n";
																				}
																			}
																			else
																			{
																				if (deathmsg == 41)
																				{
																					deathstring = " was killed by ";
																					deathstring2 = "'s Laser Drone\n";
																					if (targ->s.v.health < -40)
																					{
																						deathstring = " was vaporized by ";
																						deathstring2 = "'s Laser Drone\n";
																					}
																				}
																				else
																				{
																					if (deathmsg == 42)
																					{
																						deathstring = " was torn up by an enemy Rottweiler";
																						deathstring2 = "\n";
																					}
																					else
																					{
																						if (deathmsg == 44)
																						{
																							deathstring = " was stopped by an enemy autoturret";
																							deathstring2 = "\n";
																						}
																						else
																						{
																							if (deathmsg == 15)
																							{
																								if (rnum < 0.2)
																								{
																									deathstring = " burns to death by ";
																									deathstring2 = "'s flame\n";
																								}
																								else
																								{
																									if (rnum < 0.4)
																									{
																										deathstring = " is fried by ";
																										deathstring2 = "'s fire\n";
																									}
																									else
																									{
																										if (rnum < 0.6)
																										{
																											deathstring = " is boiled alive by ";
																											deathstring2 = "'s heat\n";
																										}
																										else
																										{
																											if (rnum < 0.8)
																											{
																												deathstring = " is cremated by ";
																												deathstring2 = "'s incinerator\n";
																											}
																											else
																											{
																												deathstring = " is grilled by ";
																												deathstring2 = "'s BBQ\n";
																											}
																										}
																									}
																								}
																							}
																							else
																							{
																								if (deathmsg == 17)
																								{
																									if (attacker->playerclass == PC_SPY)
																									{
																										deathstring = " was stabbed by ";
																									}
																									else
																									{
																										deathstring = " was slit open by ";
																									}
																									deathstring2 = "\n";
																								}
																								else
																								{
																									if (deathmsg == 32)
																									{
																										deathstring = " was spanner-whacked by ";
																										deathstring2 = "\n";
																									}
																									else
																									{
																										if (deathmsg == 1)
																										{
																											deathstring = " was swiss-cheesed by ";
																											deathstring2 = "'s bird gun\n";
//																											//shotgun_exp_gain ();
																										}
																										else
																										{
																											if (deathmsg == 2)
																											{
																												deathstring = "'s head is popped by ";
																												deathstring2 = "'s shotgun\n";
																											}
																											else
																											{
																												if (deathmsg == 3)
																												{
																													deathstring = " was capped by ";
																													deathstring2 = "\n";
																												}
																												else
																												{
																													if (deathmsg == 4)
																													{
																														deathstring = " was perforated by ";
																														deathstring2 = "'s nailgun\n";
																													}
																													else
																													{
																														if (deathmsg == 7)
																														{
																															deathstring = " swims with ";
																															deathstring2 = "'s toaster\n";
																														}
																														else
																														{
																															if (deathmsg == 21)
																															{
																																deathstring = " grappled with ";
																																deathstring2 = "\n";
																															}
																															else
																															{
																																if (deathmsg == 18)
																																{
																																	if (rnum <= 0.3)
																																	{
																																		deathstring = " gets a sucking chest wound from ";
																																		deathstring2 = "\n";
																																	}
																																	else
																																	{
																																		if (rnum <= 0.6)
																																		{
																																			deathstring = "'s liver is blown out by ";
																																			deathstring2 = "\n";
																																		}
																																		else
																																		{
																																			deathstring = "'s chest explodes from ";
																																			deathstring2 = "'s sniper round\n";
																																		}
																																	}
																																}
																																else
																																{
																																	if (deathmsg == 29)
																																	{
																																		if (rnum <= 0.5)
																																		{
																																			deathstring = " is beheaded by ";
																																			deathstring2 = "'s round\n";
																																		}
																																		else
																																		{
																																			deathstring = "'s labotomized by ";
																																			deathstring2 = "'s sniper round\n";
																																		}
																																	}
																																	else
																																	{
																																		if (deathmsg == 28)
																																		{
																																			if (rnum <= 0.5)
																																			{
																																				deathstring = " is neutered by ";
																																				deathstring2 = "\n";
																																			}
																																			else
																																			{
																																				deathstring = "'s legs explode open from ";
																																				deathstring2 = "'s shot\n";
																																			}
																																		}
																																		else
																																		{
																																			if (deathmsg == 19)
																																			{
																																				deathstring = " enjoys ";
																																				deathstring2 = "'s machinegun.\n";
																																			}
																																			else
																																			{
																																				if (deathmsg == 20)
																																				{
																																					deathstring = " torso is removed by ";
																																					deathstring2 = "\n";
																																				}
																																				else
																																				{
																																					if (deathmsg == 22)
																																					{
																																						deathstring = " is ass-knifed by ";
																																						deathstring2 = "\n";
																																						sound(targ, 0, "player/mpain6.wav", 1, 1);
																																					}
																																					else
																																					{
																																						if (deathmsg == 25)
																																						{
																																							deathstring = " is over-dosed by ";
																																							deathstring2 = "'s ludes\n";
																																						}
																																						else
																																						{
																																							if (deathmsg == 26)
																																							{
																																								deathstring = " spews juice thru holes from ";
																																								deathstring2 = "'s railgun\n";
																																							}
																																							else
																																							{
																																								if (deathmsg == 33)
																																								{
																																									deathstring = " gets cooked by ";
																																									deathstring2 = "'s incendiary rocket\n";
																																								} 
																																								// pablo. spike trap death msg.
																																								else
																																								{
																																									if (deathmsg == 200)
																																									{
																																										deathstring = "'s leg was amputated by ";
																																										deathstring2 = "'s spike\n";
																																									}
																																									// pablo. dispenser death msg.
																																									else
																																									{
																																										if (deathmsg == 201)
																																										{
																																											deathstring = " thought ";
																																											deathstring2 = "'s dispenser was a mechanical bull\n";
																																										}
																																									}
																																								}
																																							}																																						
																																						}
																																					}
																																				}
																																			}
																																		}
																																	}
																																}
																															}
																														}
																													}
																												}
																											}
																										}
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					G_bprint(1, targ->s.v.netname);
					G_bprint(1, deathstring);
					G_bprint(1, attacker->s.v.netname);
					G_bprint(1, deathstring2);
				}
			}
			return;
		}
		else
		{
			if (streq(attacker->s.v.classname, "building_sentrygun"))
			{
				if (targ == attacker->real_owner)
				{
					if (deathmsg == 34)
					{
						deathstring = " gets wasted by his sentry gun\nDumb-ass!\n";
					}
					else
					{
						if (deathmsg == 27)
						{
							deathstring = " crossed his sentry gun's line of fire\n";
						}
					}
					G_bprint(1, targ->s.v.netname);
					G_bprint(1, deathstring);
				}
				else
				{
					attacker->real_owner->real_frags = attacker->real_owner->real_frags + 1;
					logfrag(attacker->real_owner, targ);
					if (!(tf_data.toggleflags & 128))
					{
						attacker->real_owner->s.v.frags = attacker->real_owner->real_frags;
					}
					if (deathmsg == 34)
					{
						deathstring = " is creamed by ";
						deathstring2 = "'s sentry gun\n";
					}
					else
					{
						if (deathmsg == 27)
						{
							deathstring = "'s spine is extracted by ";
							deathstring2 = "'s sentry gun\n";
						}
					}
					G_bprint(1, targ->s.v.netname);
					G_bprint(1, deathstring);
					G_bprint(1, attacker->real_owner->s.v.netname);
					G_bprint(1, deathstring2);
				}
			}
			else
			{
				logfrag(targ, targ);
				targ->real_frags = targ->real_frags - 1;
				if (!(tf_data.toggleflags & 128))
				{
					targ->s.v.frags = targ->real_frags;
				}
				rnum = targ->s.v.watertype;
				G_bprint(2, targ->s.v.netname);
				if (rnum == -3)
				{
					if (g_random() < 0.5)
					{
						deathstring = " can't swim worth a crap!\n";
					}
					else
					{
						deathstring = " can't breathe water\n";
					}
				}
				else
				{
					if (rnum == -4)
					{
						if (g_random() < 0.5)
						{
							deathstring = " gulped a load of slime\n";
						}
						else
						{
							deathstring = " can't exist on slime alone\n";
						}
					}
					else
					{
						if (rnum == -5)
						{
							if (targ->s.v.health < -15)
							{
								deathstring = " burst into flames\n";
							}
							else
							{
								if (g_random() < 0.5)
								{
									deathstring = " turned into hot slag\n";
								}
								else
								{
									deathstring = " visits the Hell fires\n";
								}
							}
						}
						else
						{
							if (streq(attacker->s.v.classname, "monster_dog"))
							{
								deathstring = " was mauled by a Rottweiler\n";
							}
							else
							{
								if (streq(attacker->s.v.classname, "explo_box"))
								{
									deathstring = " blew up\n";
								}
								else
								{
									if (attacker->s.v.solid == SOLID_BSP && attacker != world)
									{
										deathstring = " was crushed\n";
									}
									else
									{
										if (targ->deathtype == "falling")
										{
											targ->deathtype = "";
											deathstring = " fell to his death\n";
										}
										else
										{
											if (streq(attacker->s.v.classname, "trap_shooter") || attacker->s.v.classname  == "trap_spikeshooter")
											{
												deathstring = " was shot\n";
											}
											else
											{
												if (streq(attacker->s.v.classname, "fireball"))
												{
													deathstring = " ate a lavaball\n";
												}
												else
												{
													if (streq(attacker->s.v.classname, "trigger_changelevel"))
													{
														deathstring = " tried to leave\n";
													}
													else
													{
														deathstring = " died\n";
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				G_bprint(1, deathstring);
			}
		}
	}
	else
	{
		if (streq(targ->s.v.classname, "building_sentrygun"))
		{
			if (streq(attacker->s.v.classname, "teledeath"))
			{
				G_bprint(1, targ->real_owner->s.v.netname);
				G_bprint(1, "'s sentrygun was telefragged by ");
				G_bprint(1, PROG_TO_EDICT(attacker->s.v.owner)->s.v.netname);
				G_bprint(1, "\n");
				return;
			}
			if (streq(attacker->s.v.classname, "player"))
			{
				if (attacker == targ->real_owner)
				{
					G_bprint(1, targ->real_owner->s.v.netname);
					G_bprint(1, " selfdistructs his sentrygun\n");
					return;
				}
				G_bprint(1, targ->real_owner->s.v.netname);
				G_bprint(1, "'s sentrygun was no match for ");
				G_bprint(1, attacker->s.v.netname);
				G_bprint(1, "\n");
				logfrag(attacker, targ->real_owner);
				attacker->real_frags = attacker->real_frags + 1;
				// pablo. no frags when teamscores is on.
				if (!(tf_data.toggleflags & 128))
				{
					attacker->s.v.frags = attacker->real_frags;
				}
				//
				if (attacker->team_no > 0 && attacker->team_no != targ->real_owner->team_no)
				{
					attacker->real_frags = attacker->real_frags + 1;
					logfrag(attacker, targ->real_owner);
					if (!(tf_data.toggleflags & 128))
					{
						attacker->s.v.frags = attacker->real_frags;
					}
				}
				return;
			}
		}
	}
}