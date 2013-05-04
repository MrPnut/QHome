Title:		MegaTF (2006) C Project
Author:		Team Fortress team, SD Angel, and XavioR (aka avirox)
Release Date:	November 2, 2006
Information:	The goal of the MegaTF C project is to convert the popular megatf
		(Mega Team Fortress) mod to the C language. Based on the updated
		Team Fortress 2003 code, MegaTF C (codename MegaTF 2006) features
		bug fixes and updated entities. Also, being compiled in C, it has
		several speed advantages over its' predecessor. Above all however,
		the source code can legitimately be released publicly, and is not
		in the messy form of the decompiled code.

=================================
	MegaTF - C
=================================

		[Compiling]

	The DLL binary (qwprogs.dll) is compiled to the "Release" folder.
	I've improved tf2003's VM compiler batch file to do everything for
	you incase you use in windows. Just execute it and it will compile
	the VM to the "VM" folder (qwprogs.qvm). You can also compile a .so
	with GCC for linux based OSs.

		[Making the server]

	Firstly, take the compiled VM (qwprogs.blah) and place it in /quake/fortress/.
	If you plan on running a windows server, the DLL would be the fastest
	VM to use. QVM can also be run under windows but is a little slower (though
	still faster than the qwprogs.dll). To run the MTF-C server, load MVDSV using:

	In Windows:
	For DLL:
	mvdsv<insert version info here>.exe +gamedir fortress -progtype 1

	For QVM:
	mvdsv<insert version info here>.exe +gamedir fortress -progtype 2 -mem 32 (mem part is optional - only use it if you get errors, and adjust accordingly)

	In Linux/FreeBSD/etc:
	For QVM or SO:
	mvdsv<insert version info here> +gamedir fortress -progtype 2

	All other server functions are the same as they were with any other mtf release.

		[Known Bugs/Issues]

	Well there are still a few rough edges here and there. I'll list the ones I can
	remember off hand. Note that they're nothing major and don't really affect gameplay.
	
	1 - Sometimes the client info on player join is not displayed correctly
	2 - There are some missing entities (such as the mega turret). These entities
	may appear in later versions, but I am not sure on my plans there. I'm hoping
	someone will come along and add these. Sandball and some other basic MTF entities
	are in though.
	3 - dropflag and flag info arent in. Dropitems and all tf2003 related features
	are in though (I dont know all of these, so read the tf2003 docs)
	4 - The menu when you select a class is annoying. ;)
	5 - Player frags are static. SD Angel tells me this has something to do with
	tf2003's clan mode.
	6 - Random non-gameplay elements that were added by plexi/death/plasticity/etc
	are not in (look at the 11.11.04 changelogs and compare if you want..).

	There's probably more, but nothing that I can remember at the moment. This mod
	is still fully playable however.
	

		[Other Notes]

	This project is as much for me as for the MTF/TF/Quake modding community. This
	is the first project that I have ever done in C, and therefore is bound to look
	ugly and be a little buggy. I did this in part to help me learn more about the
	C language (and thanks to the help of SD Angel, I think I know atleast a little 
	more than I used to..), and in part to see MegaTF released in a more legible form.
	I hope that interested modders will take this mod and do what they like with it,
	adding mods or updating/fixing it etc. Enjoy!

	

	E-mail me at avirox38@hotmail.com with questions/comments!


			"Art is the truth-telling lie."