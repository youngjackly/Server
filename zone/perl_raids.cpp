/*
* This file was generated automatically by ExtUtils::ParseXS version 2.18 from the
* contents of raids.h.xs. Do not edit this file, edit raids.h.xs instead.
*
*	ANY CHANGES MADE HERE WILL BE LOST!
*
*/


/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2004 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "../common/gamefeatures.h"
#ifdef EMBPERL_XS_CLASSES
#include "../common/global_define.h"
#include "embperl.h"

#ifdef seed
#undef seed
#endif

#include "raids.h"
#include "client.h"

#ifdef THIS /* this macro seems to leak out on some systems */
#undef THIS
#endif


XS(XS_Raid_IsRaidMember); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_IsRaidMember)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::IsRaidMember(THIS, name)");
	{
		Raid *		THIS;
		bool		RETVAL;
		const char*	name = (char *)SvPV_nolen(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->IsRaidMember(name);
		ST(0) = boolSV(RETVAL);
		sv_2mortal(ST(0));
	}
	XSRETURN(1);
}

XS(XS_Raid_CastGroupSpell); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_CastGroupSpell)
{
	dXSARGS;
	if (items != 4)
		Perl_croak(aTHX_ "Usage: Raid::CastGroupSpell(THIS, caster, spellid, gid)");
	{
		Raid *		THIS;
		Mob*		caster;
		uint16		spellid = (uint16)SvUV(ST(2));
		uint32		gid = (uint32)SvUV(ST(3));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		if (sv_derived_from(ST(1), "Mob")) {
			IV tmp = SvIV((SV*)SvRV(ST(1)));
			caster = INT2PTR(Mob *,tmp);
		}
		else
			Perl_croak(aTHX_ "caster is not of type Mob");
		if(caster == nullptr)
			Perl_croak(aTHX_ "caster is nullptr, avoiding crash.");

		THIS->CastGroupSpell(caster, spellid, gid);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_GroupCount); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GroupCount)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::GroupCount(THIS, gid)");
	{
		Raid *		THIS;
		uint8		RETVAL;
		dXSTARG;
		uint32		gid = (uint32)SvUV(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GroupCount(gid);
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_RaidCount); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_RaidCount)
{
	dXSARGS;
	if (items != 1)
		Perl_croak(aTHX_ "Usage: Raid::RaidCount(THIS)");
	{
		Raid *		THIS;
		uint8		RETVAL;
		dXSTARG;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->RaidCount();
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_GetGroup); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetGroup)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::GetGroup(THIS, name)");
	{
		Raid *		THIS;
		uint32		RETVAL;
		dXSTARG;
		const char*	name = (char *)SvPV_nolen(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GetGroup(name);
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_SplitExp); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_SplitExp)
{
	dXSARGS;
	if (items != 3)
		Perl_croak(aTHX_ "Usage: Raid::SplitExp(THIS, exp, other)");
	{
		Raid *		THIS;
		uint32		exp = (uint32)SvUV(ST(1));
		Mob*		other;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		if (sv_derived_from(ST(2), "Mob")) {
			IV tmp = SvIV((SV*)SvRV(ST(2)));
			other = INT2PTR(Mob *,tmp);
		}
		else
			Perl_croak(aTHX_ "other is not of type Mob");
		if(other == nullptr)
			Perl_croak(aTHX_ "other is nullptr, avoiding crash.");

		THIS->SplitExp(exp, other);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_GetTotalRaidDamage); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetTotalRaidDamage)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::GetTotalRaidDamage(THIS, other)");
	{
		Raid *		THIS;
		uint32		RETVAL;
		dXSTARG;
		Mob*		other;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		if (sv_derived_from(ST(1), "Mob")) {
			IV tmp = SvIV((SV*)SvRV(ST(1)));
			other = INT2PTR(Mob *,tmp);
		}
		else
			Perl_croak(aTHX_ "other is not of type Mob");
		if(other == nullptr)
			Perl_croak(aTHX_ "other is nullptr, avoiding crash.");

		RETVAL = THIS->GetTotalRaidDamage(other);
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_SplitMoney); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_SplitMoney)
{
	dXSARGS;
	if (items != 5)
		Perl_croak(aTHX_ "Usage: Raid::SplitMoney(THIS, copper, silver, gold, platinum)");
	{
		Raid *		THIS;
		uint32		copper = (uint32)SvUV(ST(1));
		uint32		silver = (uint32)SvUV(ST(2));
		uint32		gold = (uint32)SvUV(ST(3));
		uint32		platinum = (uint32)SvUV(ST(4));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		THIS->SplitMoney(copper, silver, gold, platinum);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_BalanceHP); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_BalanceHP)
{
	dXSARGS;
	if (items != 3)
		Perl_croak(aTHX_ "Usage: Raid::BalanceHP(THIS, penalty, gid)");
	{
		Raid *		THIS;
		int32		penalty = (int32)SvUV(ST(1));
		uint32		gid = (uint32)SvUV(ST(2));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		THIS->BalanceHP(penalty, gid);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_IsLeader); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_IsLeader)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::IsLeader(THIS, name)");
	{
		Raid *		THIS;
		bool		RETVAL;
		const char*	name = (char *)SvPV_nolen(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->IsLeader(name);
		ST(0) = boolSV(RETVAL);
		sv_2mortal(ST(0));
	}
	XSRETURN(1);
}

XS(XS_Raid_IsGroupLeader); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_IsGroupLeader)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::IsGroupLeader(THIS, who)");
	{
		Raid *		THIS;
		bool		RETVAL;
		const char*	who = (char *)SvPV_nolen(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->IsGroupLeader(who);
		ST(0) = boolSV(RETVAL);
		sv_2mortal(ST(0));
	}
	XSRETURN(1);
}

XS(XS_Raid_GetHighestLevel); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetHighestLevel)
{
	dXSARGS;
	if (items != 1)
		Perl_croak(aTHX_ "Usage: Raid::GetHighestLevel(THIS)");
	{
		Raid *		THIS;
		uint32		RETVAL;
		dXSTARG;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GetHighestLevel();
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_GetLowestLevel); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetLowestLevel)
{
	dXSARGS;
	if (items != 1)
		Perl_croak(aTHX_ "Usage: Raid::GetLowestLevel(THIS)");
	{
		Raid *		THIS;
		uint32		RETVAL;
		dXSTARG;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GetLowestLevel();
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_GetClientByIndex); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetClientByIndex)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::GetClientByIndex(THIS, index)");
	{
		Raid *		THIS;
		Client *		RETVAL;
		uint16		index = (uint16)SvUV(ST(1));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GetClientByIndex(index);
		ST(0) = sv_newmortal();
		sv_setref_pv(ST(0), "Client", (void*)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_TeleportGroup); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_TeleportGroup)
{
	dXSARGS;
	if (items != 8)
		Perl_croak(aTHX_ "Usage: Raid::TeleportGroup(THIS, sender, zoneID, x, y, z, heading, gid)");
	{
		Raid *		THIS;
		Mob*		sender;
		uint32		zoneID = (uint32)SvUV(ST(2));
		float		x = (float)SvNV(ST(3));
		float		y = (float)SvNV(ST(4));
		float		z = (float)SvNV(ST(5));
		float		heading = (float)SvNV(ST(6));
		uint32		gid = (uint32)SvUV(ST(7));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		if (sv_derived_from(ST(1), "Mob")) {
			IV tmp = SvIV((SV*)SvRV(ST(1)));
			sender = INT2PTR(Mob *,tmp);
		}
		else
			Perl_croak(aTHX_ "sender is not of type Mob");
		if(sender == nullptr)
			Perl_croak(aTHX_ "sender is nullptr, avoiding crash.");

		THIS->TeleportGroup(sender, zoneID, 0, x, y, z, heading, gid);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_TeleportRaid); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_TeleportRaid)
{
	dXSARGS;
	if (items != 7)
		Perl_croak(aTHX_ "Usage: Raid::TeleportRaid(THIS, sender, zoneID, x, y, z, heading)");
	{
		Raid *		THIS;
		Mob*		sender;
		uint32		zoneID = (uint32)SvUV(ST(2));
		float		x = (float)SvNV(ST(3));
		float		y = (float)SvNV(ST(4));
		float		z = (float)SvNV(ST(5));
		float		heading = (float)SvNV(ST(6));

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		if (sv_derived_from(ST(1), "Mob")) {
			IV tmp = SvIV((SV*)SvRV(ST(1)));
			sender = INT2PTR(Mob *,tmp);
		}
		else
			Perl_croak(aTHX_ "sender is not of type Mob");
		if(sender == nullptr)
			Perl_croak(aTHX_ "sender is nullptr, avoiding crash.");

		THIS->TeleportRaid(sender, zoneID, 0, x, y, z, heading);
	}
	XSRETURN_EMPTY;
}

XS(XS_Raid_GetID); /* prototype to pass -Wmissing-prototypes */
XS(XS_Raid_GetID)
{
	dXSARGS;
	if (items != 1)
		Perl_croak(aTHX_ "Usage: Raid::GetID(THIS)");
	{
		Raid *		THIS;
		uint32		RETVAL;
		dXSTARG;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		RETVAL = THIS->GetID();
		XSprePUSH; PUSHu((UV)RETVAL);
	}
	XSRETURN(1);
}

XS(XS_Raid_GetMember);
XS(XS_Raid_GetMember)
{
	dXSARGS;
	if (items != 2)
		Perl_croak(aTHX_ "Usage: Raid::GetMember(THIS, index)");
	{
		Raid * THIS;
		Client*	RETVAL = nullptr;
		dXSTARG;

		if (sv_derived_from(ST(0), "Raid")) {
			IV tmp = SvIV((SV*)SvRV(ST(0)));
			THIS = INT2PTR(Raid *,tmp);
		}
		else
			Perl_croak(aTHX_ "THIS is not of type Raid");
		if(THIS == nullptr)
			Perl_croak(aTHX_ "THIS is nullptr, avoiding crash.");

		int index = (int)SvUV(ST(1));
		if (index < 0 || index > 71)
			RETVAL = nullptr;
		else {
			if(THIS->members[index].member != nullptr)
				RETVAL = THIS->members[index].member->CastToClient();
		}

		ST(0) = sv_newmortal();
		sv_setref_pv(ST(0), "Client", (void*)RETVAL);
	}
	XSRETURN(1);
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_Raid); /* prototype to pass -Wmissing-prototypes */
XS(boot_Raid)
{
	dXSARGS;
	char file[256];
	strncpy(file, __FILE__, 256);
	file[255] = 0;

	if(items != 1)
		fprintf(stderr, "boot_quest does not take any arguments.");
	char buf[128];

	//add the strcpy stuff to get rid of const warnings....

	XS_VERSION_BOOTCHECK ;

		newXSproto(strcpy(buf, "IsRaidMember"), XS_Raid_IsRaidMember, file, "$$");
		newXSproto(strcpy(buf, "CastGroupSpell"), XS_Raid_CastGroupSpell, file, "$$$$");
		newXSproto(strcpy(buf, "GroupCount"), XS_Raid_GroupCount, file, "$$");
		newXSproto(strcpy(buf, "RaidCount"), XS_Raid_RaidCount, file, "$");
		newXSproto(strcpy(buf, "GetGroup"), XS_Raid_GetGroup, file, "$$");
		newXSproto(strcpy(buf, "SplitExp"), XS_Raid_SplitExp, file, "$$$");
		newXSproto(strcpy(buf, "GetTotalRaidDamage"), XS_Raid_GetTotalRaidDamage, file, "$$");
		newXSproto(strcpy(buf, "SplitMoney"), XS_Raid_SplitMoney, file, "$$$$$");
		newXSproto(strcpy(buf, "BalanceHP"), XS_Raid_BalanceHP, file, "$$$");
		newXSproto(strcpy(buf, "IsLeader"), XS_Raid_IsLeader, file, "$$");
		newXSproto(strcpy(buf, "IsGroupLeader"), XS_Raid_IsGroupLeader, file, "$$");
		newXSproto(strcpy(buf, "GetHighestLevel"), XS_Raid_GetHighestLevel, file, "$");
		newXSproto(strcpy(buf, "GetLowestLevel"), XS_Raid_GetLowestLevel, file, "$");
		newXSproto(strcpy(buf, "GetClientByIndex"), XS_Raid_GetClientByIndex, file, "$$");
		newXSproto(strcpy(buf, "TeleportGroup"), XS_Raid_TeleportGroup, file, "$$$$$$$$");
		newXSproto(strcpy(buf, "TeleportRaid"), XS_Raid_TeleportRaid, file, "$$$$$$$");
		newXSproto(strcpy(buf, "GetID"), XS_Raid_GetID, file, "$");
		newXSproto(strcpy(buf, "GetMember"), XS_Raid_GetMember, file, "$$");
	XSRETURN_YES;
}

#endif //EMBPERL_XS_CLASSES

