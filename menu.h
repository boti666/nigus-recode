#pragma once

class WeaponCfg {
public:
	MultiDropdown hitbox;
	MultiDropdown hitbox_history;
	MultiDropdown multipoint;
	Slider		  scale;
	Slider		  body_scale;
	Slider		  minimal_damage;
	Checkbox	  penetrate;
	Slider		  penetrate_minimal_damage;
	Checkbox	  minimal_damage_hp;

	Checkbox      hitchance;
	Slider	      hitchance_amount;
	MultiDropdown baim1;
	MultiDropdown baim2;
	MultiDropdown increasefirerate;
	Slider        baim_hp;
};

class AimbotTab : public Tab {
public:
	// col1.
	Checkbox	  enable;
	Dropdown	  selection;
	MultiDropdown aimbobs;

	Button        refresh_seeds;

	WeaponCfg     general;
	WeaponCfg     auto_sniper;
	WeaponCfg     awp;
	WeaponCfg     scout;
	WeaponCfg     heavy_pistol;
	WeaponCfg     pistol;

	// col2.
	Dropdown      zoom;
	Checkbox      nospread;
	Dropdown      lagfix;
	Keybind       baim_key;
	Keybind       headglitch_key;
	Keybind       dmg_override;
	Slider        dmg_override_amount;
	Dropdown	  correct;
	Keybind	      correct_override;
	Slider        correct_threshold;

	Dropdown      shot_matrix;
	Colorpicker   shot_matrix_color;
	Dropdown      shot_matrix_material;

	Dropdown	  tab;
	Dropdown	  weapon;
public:
	void init() {
		// title.
		SetTitle(XOR("aimbot"));

		enable.setup(XOR("enable"), XOR("enable"));
		RegisterElement(&enable);

		tab.setup(XOR(""), XOR("tab"), { XOR("general"), XOR("weapons") }, false);
		RegisterElement(&tab);

		selection.setup(XOR("target selection"), XOR("selection"), { XOR("distance"), XOR("crosshair"), XOR("damage"), XOR("health"), XOR("lag"), XOR("height"), XOR("resolver") });
		selection.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&selection);

		aimbobs.setup(XOR("features"), XOR("features"), { XOR("silent aimbot"), XOR("taser bot"), XOR("retreat with knife"), XOR("compensate recoil"), XOR("automatic stop")});
		aimbobs.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&aimbobs);


		weapon.setup(XOR("weapon"), XOR("weapon"), { XOR("general"), XOR("auto"), XOR("awp"), XOR("scout"), XOR("heavy pistol"), XOR("pistol") });
		weapon.AddShowCallback(callbacks::IsTabWeaponAimbot);
		RegisterElement(&weapon);



		// general
		general.hitbox.setup(XOR("hitbox"), XOR("hitbox_general"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		general.hitbox.AddShowCallback(callbacks::IsGeneral);
		general.hitbox.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.hitbox);

		general.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_general"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		general.hitbox_history.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.hitbox_history);

		general.multipoint.setup(XOR("multi-point"), XOR("multipoint_general"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		general.multipoint.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.multipoint);

		general.scale.setup("", XOR("hitbox_scale_general"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		general.scale.AddShowCallback(callbacks::IsGeneral);
		general.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&general.scale);

		general.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_general"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		general.body_scale.AddShowCallback(callbacks::IsGeneral);
		general.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&general.body_scale);

		general.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_general"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		general.minimal_damage.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.minimal_damage);

		general.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_general"));
		general.penetrate.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.penetrate);

		general.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_general"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		general.penetrate_minimal_damage.AddShowCallback(callbacks::IsGeneral);
		general.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&general.penetrate_minimal_damage);

		general.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_general"));
		general.minimal_damage_hp.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.minimal_damage_hp);

		// auto
		auto_sniper.hitbox.setup(XOR("hitbox"), XOR("hitbox_auto"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		auto_sniper.hitbox.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.hitbox);

		auto_sniper.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_auto"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		auto_sniper.hitbox_history.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.hitbox_history);

		auto_sniper.multipoint.setup(XOR("multi-point"), XOR("multipoint_auto"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		auto_sniper.multipoint.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.multipoint);

		auto_sniper.scale.setup("", XOR("hitbox_scale_auto"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		auto_sniper.scale.AddShowCallback(callbacks::IsAuto);
		auto_sniper.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&auto_sniper.scale);

		auto_sniper.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_auto"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		auto_sniper.body_scale.AddShowCallback(callbacks::IsAuto);
		auto_sniper.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&auto_sniper.body_scale);

		auto_sniper.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_auto"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		auto_sniper.minimal_damage.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.minimal_damage);

		auto_sniper.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_auto"));
		auto_sniper.penetrate.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.penetrate);

		auto_sniper.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_auto"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		auto_sniper.penetrate_minimal_damage.AddShowCallback(callbacks::IsAuto);
		auto_sniper.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&auto_sniper.penetrate_minimal_damage);

		auto_sniper.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_auto"));
		auto_sniper.minimal_damage_hp.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.minimal_damage_hp);
		//awp










		awp.hitbox.setup(XOR("hitbox"), XOR("hitbox_awp"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		awp.hitbox.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.hitbox);

		awp.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_awp"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		awp.hitbox_history.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.hitbox_history);

		awp.multipoint.setup(XOR("multi-point"), XOR("multipoint_awp"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		awp.multipoint.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.multipoint);

		awp.scale.setup("", XOR("hitbox_scale_awp"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		awp.scale.AddShowCallback(callbacks::IsAwp);
		awp.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&awp.scale);

		awp.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_awp"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		awp.body_scale.AddShowCallback(callbacks::IsAwp);
		awp.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&awp.body_scale);

		awp.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_awp"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		awp.minimal_damage.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.minimal_damage);

		awp.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_awp"));
		awp.penetrate.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.penetrate);

		awp.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_awp"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		awp.penetrate_minimal_damage.AddShowCallback(callbacks::IsAwp);
		awp.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&awp.penetrate_minimal_damage);

		awp.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_awp"));
		awp.minimal_damage_hp.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.minimal_damage_hp);

		// scout
		scout.hitbox.setup(XOR("hitbox"), XOR("hitbox_scout"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		scout.hitbox.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.hitbox);

		scout.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_scout"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		scout.hitbox_history.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.hitbox_history);

		scout.multipoint.setup(XOR("multi-point"), XOR("multipoint_scout"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		scout.multipoint.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.multipoint);

		scout.scale.setup("", XOR("hitbox_scale_scout"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		scout.scale.AddShowCallback(callbacks::IsScout);
		scout.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&scout.scale);

		scout.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_scout"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		scout.body_scale.AddShowCallback(callbacks::IsScout);
		scout.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&scout.body_scale);

		scout.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_scout"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		scout.minimal_damage.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.minimal_damage);

		scout.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_scout"));
		scout.penetrate.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.penetrate);

		scout.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_scout"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		scout.penetrate_minimal_damage.AddShowCallback(callbacks::IsScout);
		scout.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&scout.penetrate_minimal_damage);

		scout.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_scout"));
		scout.minimal_damage_hp.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.minimal_damage_hp);

		//hpistol
		heavy_pistol.hitbox.setup(XOR("hitbox"), XOR("hitbox_heavy_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		heavy_pistol.hitbox.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.hitbox);

		heavy_pistol.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_heavy_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		heavy_pistol.hitbox_history.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.hitbox_history);

		heavy_pistol.multipoint.setup(XOR("multi-point"), XOR("multipoint_heavy_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		heavy_pistol.multipoint.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.multipoint);

		heavy_pistol.scale.setup("", XOR("hitbox_scale_heavy_pistol"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		heavy_pistol.scale.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&heavy_pistol.scale);

		heavy_pistol.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_heavy_pistol"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		heavy_pistol.body_scale.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&heavy_pistol.body_scale);

		heavy_pistol.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_heavy_pistol"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		heavy_pistol.minimal_damage.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.minimal_damage);

		heavy_pistol.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_heavy_pistol"));
		heavy_pistol.penetrate.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.penetrate);

		heavy_pistol.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_heavy_pistol"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		heavy_pistol.penetrate_minimal_damage.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&heavy_pistol.penetrate_minimal_damage);

		heavy_pistol.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_heavy_pistol"));
		heavy_pistol.minimal_damage_hp.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.minimal_damage_hp);

		// pistol
		pistol.hitbox.setup(XOR("hitbox"), XOR("hitbox_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		pistol.hitbox.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.hitbox);

		pistol.hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		pistol.hitbox_history.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.hitbox_history);

		pistol.multipoint.setup(XOR("multi-point"), XOR("multipoint_pistol"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		pistol.multipoint.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.multipoint);

		pistol.scale.setup("", XOR("hitbox_scale_pistol"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		pistol.scale.AddShowCallback(callbacks::IsPistol);
		pistol.scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&pistol.scale);

		pistol.body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale_pistol"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		pistol.body_scale.AddShowCallback(callbacks::IsPistol);
		pistol.body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&pistol.body_scale);

		pistol.minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage_pistol"), 1.f, 130.f, true, 0, 40.f, 1.f, XOR(L"hp"), true);
		pistol.minimal_damage.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.minimal_damage);

		pistol.penetrate.setup(XOR("penetrate walls"), XOR("penetrate_pistol"));
		pistol.penetrate.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.penetrate);

		pistol.penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage_pistol"), 1.f, 130.f, false, 0, 30.f, 1.f, XOR(L"hp"), true);
		pistol.penetrate_minimal_damage.AddShowCallback(callbacks::IsPistol);
		pistol.penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&pistol.penetrate_minimal_damage);

		pistol.minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp_pistol"));
		pistol.minimal_damage_hp.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.minimal_damage_hp);


		shot_matrix.setup(XOR("shot matrix"), XOR("shot_matrix"), { XOR("off"), XOR("full body") });
		shot_matrix.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&shot_matrix);

		shot_matrix_color.setup(XOR(""), XOR("shot_matrix_color"), colors::white, nullptr, true);
		shot_matrix_color.AddShowCallback(callbacks::IsTabGeneralAimbot);
		shot_matrix_color.AddShowCallback(callbacks::ShotmatrixOn1);
		RegisterElement(&shot_matrix_color);



		// col2.
		zoom.setup(XOR("auto scope"), XOR("zoom"), { XOR("off"), XOR("always"), XOR("hitchance fail") });
		zoom.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&zoom, 1);

		nospread.setup(XOR("compensate spread"), XOR("nospread"));
		nospread.AddShowCallback(callbacks::IsConfigNS);
		nospread.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&nospread, 1);


		lagfix.setup(XOR("predict fakelag"), XOR("lagfix"), { XOR("off"), XOR("predict")/*, XOR("always")*/ });
		lagfix.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&lagfix, 1);

		correct.setup(XOR("correct anti-aim"), XOR("correct"), { XOR("none"), XOR("normal"), XOR("extend") });
		correct.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&correct, 1);

		headglitch_key.setup(XOR("headglitch"), XOR("headglitch aimbot"));
		headglitch_key.SetToggleCallback(callbacks::HeadGlitchAimbot);
		headglitch_key.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&headglitch_key, 1);

		correct_threshold.setup(XOR("correction treshold"), XOR("correct_threshold"), 15.f, 90.f, true, 0, 45.f, 1, L"º");
		correct_threshold.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&correct_threshold, 1);

		correct_override.setup(XOR("override correction"), XOR("correct_override"));
		correct_override.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&correct_override, 1);

		general.increasefirerate.setup(XOR("increase fire-rate"), XOR("increasefirerate_general"), { XOR("always"), XOR("target on land"), XOR("target moving"), XOR("target in air") });
		general.increasefirerate.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&general.increasefirerate, 1);

		//general
		general.hitchance.setup(XOR("hitchance"), XOR("hitchance_general"));
		general.hitchance.AddShowCallback(callbacks::IsGeneral);
		general.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&general.hitchance, 1);

		general.hitchance_amount.setup("", XOR("hitchance_amount_general"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		general.hitchance_amount.AddShowCallback(callbacks::IsGeneral);
		general.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		general.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&general.hitchance_amount, 1);


		general.baim1.setup(XOR("prefer body aim"), XOR("baim1_general"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		general.baim1.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.baim1, 1);

		general.baim2.setup(XOR("only body aim"), XOR("baim2_general"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		general.baim2.AddShowCallback(callbacks::IsGeneral);
		RegisterElement(&general.baim2, 1);

		general.baim_hp.setup("", XOR("baim_hp_general"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		general.baim_hp.AddShowCallback(callbacks::IsGeneral);
		general.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&general.baim_hp, 1);

		//auto
		auto_sniper.hitchance.setup(XOR("hitchance"), XOR("hitchance_auto"));
		auto_sniper.hitchance.AddShowCallback(callbacks::IsAuto);
		auto_sniper.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&auto_sniper.hitchance, 1);

		auto_sniper.hitchance_amount.setup("", XOR("hitchance_amount_auto"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		auto_sniper.hitchance_amount.AddShowCallback(callbacks::IsAuto);
		auto_sniper.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		auto_sniper.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&auto_sniper.hitchance_amount, 1);

		auto_sniper.baim1.setup(XOR("prefer body aim"), XOR("baim1_auto"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		auto_sniper.baim1.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.baim1, 1);

		auto_sniper.baim2.setup(XOR("only body aim"), XOR("baim2_auto"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		auto_sniper.baim2.AddShowCallback(callbacks::IsAuto);
		RegisterElement(&auto_sniper.baim2, 1);

		auto_sniper.baim_hp.setup("", XOR("baim_hp_auto"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		auto_sniper.baim_hp.AddShowCallback(callbacks::IsAuto);
		auto_sniper.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&auto_sniper.baim_hp, 1);

		//awp
		awp.hitchance.setup(XOR("hitchance"), XOR("hitchance_awp"));
		awp.hitchance.AddShowCallback(callbacks::IsAwp);
		awp.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&awp.hitchance, 1);

		awp.hitchance_amount.setup("", XOR("hitchance_amount_awp"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		awp.hitchance_amount.AddShowCallback(callbacks::IsAwp);
		awp.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		awp.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&awp.hitchance_amount, 1);

		awp.baim1.setup(XOR("prefer body aim"), XOR("baim1_awp"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		awp.baim1.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.baim1, 1);

		awp.baim2.setup(XOR("only body aim"), XOR("baim2_awp"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		awp.baim2.AddShowCallback(callbacks::IsAwp);
		RegisterElement(&awp.baim2, 1);

		awp.baim_hp.setup("", XOR("baim_hp_awp"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		awp.baim_hp.AddShowCallback(callbacks::IsAwp);
		awp.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&awp.baim_hp, 1);

		//scout
		scout.hitchance.setup(XOR("hitchance"), XOR("hitchance_scout"));
		scout.hitchance.AddShowCallback(callbacks::IsScout);
		scout.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&scout.hitchance, 1);

		scout.hitchance_amount.setup("", XOR("hitchance_amount_scout"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		scout.hitchance_amount.AddShowCallback(callbacks::IsScout);
		scout.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		scout.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&scout.hitchance_amount, 1);

		scout.baim1.setup(XOR("prefer body aim"), XOR("baim1_scout"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		scout.baim1.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.baim1, 1);

		scout.baim2.setup(XOR("only body aim"), XOR("baim2_scout"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		scout.baim2.AddShowCallback(callbacks::IsScout);
		RegisterElement(&scout.baim2, 1);

		scout.baim_hp.setup("", XOR("baim_hp_scout"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		scout.baim_hp.AddShowCallback(callbacks::IsScout);
		scout.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&scout.baim_hp, 1);

		//hpistol
		heavy_pistol.hitchance.setup(XOR("hitchance"), XOR("hitchance_heavy_pistol"));
		heavy_pistol.hitchance.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&heavy_pistol.hitchance, 1);

		heavy_pistol.hitchance_amount.setup("", XOR("hitchance_amount_heavy_pistol"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		heavy_pistol.hitchance_amount.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		heavy_pistol.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&heavy_pistol.hitchance_amount, 1);

		heavy_pistol.baim1.setup(XOR("prefer body aim"), XOR("baim1_heavy_pistol"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		heavy_pistol.baim1.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.baim1, 1);

		heavy_pistol.baim2.setup(XOR("only body aim"), XOR("baim2_heavy_pistol"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		heavy_pistol.baim2.AddShowCallback(callbacks::IsHPistol);
		RegisterElement(&heavy_pistol.baim2, 1);

		heavy_pistol.baim_hp.setup("", XOR("baim_hp_heavy_pistol"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		heavy_pistol.baim_hp.AddShowCallback(callbacks::IsHPistol);
		heavy_pistol.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&heavy_pistol.baim_hp, 1);

		//pistol
		pistol.hitchance.setup(XOR("hitchance"), XOR("hitchance_pistol"));
		pistol.hitchance.AddShowCallback(callbacks::IsPistol);
		pistol.hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&pistol.hitchance, 1);

		pistol.hitchance_amount.setup("", XOR("hitchance_amount_pistol"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		pistol.hitchance_amount.AddShowCallback(callbacks::IsPistol);
		pistol.hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		pistol.hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&pistol.hitchance_amount, 1);

		pistol.baim1.setup(XOR("prefer body aim"), XOR("baim1_pistol"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		pistol.baim1.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.baim1, 1);

		pistol.baim2.setup(XOR("only body aim"), XOR("baim2_pistol"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		pistol.baim2.AddShowCallback(callbacks::IsPistol);
		RegisterElement(&pistol.baim2, 1);

		pistol.baim_hp.setup("", XOR("baim_hp_pistol"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		pistol.baim_hp.AddShowCallback(callbacks::IsPistol);
		pistol.baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&pistol.baim_hp, 1);

		dmg_override.setup(XOR("damage override on key"), XOR("dmg_override"));
		dmg_override.SetToggleCallback(callbacks::UpdateOverride);
		dmg_override.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&dmg_override, 1);

		dmg_override_amount.setup("", XOR("dmg_override_amount"), 1.f, 130.f, false, 0, 20.f, 1.f, XOR(L"hp"), true);
		dmg_override_amount.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&dmg_override_amount, 1);

		baim_key.setup(XOR("body aim on key"), XOR("body aim on key"));
		baim_key.SetToggleCallback(callbacks::UpdateBaim);
		baim_key.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&baim_key, 1);

		refresh_seeds.setup(XOR("refresh seeds"));
		refresh_seeds.SetCallback(callbacks::RefreshSeeds);
		refresh_seeds.AddShowCallback(callbacks::IsTabGeneralAimbot);
		RegisterElement(&refresh_seeds);
	}
};

class AntiAimTab : public Tab {
public:
	Dropdown tab;
	// col 1.
	Checkbox enable;
	Checkbox edge;
	Checkbox disable_rotation_manual;
	Dropdown mode;

	Dropdown pitch_stand;
	Dropdown yaw_stand;
	Slider   jitter_range_stand;
	Slider   rot_range_stand;
	Slider   rot_speed_stand;
	Slider   rand_update_stand;
	Dropdown dir_stand;
	Slider   dir_time_stand;
	Slider   dir_custom_stand;
	Checkbox dir_lock;
	Dropdown base_angle_stand;
	Checkbox disable_body_fake_fakewalk;
	Checkbox disable_body_fake_manual;
	Checkbox body_fake_twist;
	Checkbox body_fake_stand;
	Slider   body_fake_angle_stand;
	Slider   yaw_add_stand;

	Dropdown pitch_walk;
	Dropdown yaw_walk;
	Slider   jitter_range_walk;
	Slider   rot_range_walk;
	Slider   rot_speed_walk;
	Slider   rand_update_walk;
	Dropdown dir_walk;
	Slider	 dir_time_walk;
	Slider   dir_custom_walk;
	Dropdown base_angle_walk;
	Slider   yaw_add_walk;

	Dropdown pitch_air;
	Dropdown yaw_air;
	Slider   jitter_range_air;
	Slider   rot_range_air;
	Slider   rot_speed_air;
	Slider   rand_update_air;
	Dropdown dir_air;
	Slider   dir_time_air;
	Slider   dir_custom_air;
	Dropdown base_angle_air;
	Slider   yaw_add_air;

	Keybind  manual_front;
	Keybind  manual_back;
	Keybind  manual_right;
	Keybind  manual_left;
	Checkbox desync_foot_yaw;
	Dropdown fake_flick_mode;
	Keybind  fake_flick;
	Slider   fake_flick_range;
	Colorpicker fake_flick_color;

	Keybind  inverter;

	// col 2.
	Dropdown fake_yaw;
	Slider	 fake_relative;
	Slider	 fake_jitter_range;

	Checkbox      lag_enable;
	MultiDropdown lag_active;
	Dropdown      lag_mode;
	Slider        lag_limit;
	Slider	      fluc_limit;
	Checkbox      lag_shot;
	Checkbox      lag_peek;
	Checkbox      lag_land;
	Keybind       lag_disable;

public:
	void init() {
		SetTitle(XOR("anti-aim"));

		enable.setup(XOR("enable"), XOR("enable"));
		RegisterElement(&enable);

		tab.setup(XOR(""), XOR("tab"), { XOR("general"), XOR("fake") }, false);
		RegisterElement(&tab);

		edge.setup(XOR("edge"), XOR("edge"));
		edge.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&edge);

		mode.setup("", XOR("mode"), { XOR("stand"), XOR("walk"), XOR("air") }, false);
		mode.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&mode);

		// stand.
		pitch_stand.setup(XOR("pitch"), XOR("pitch_stnd"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		pitch_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&pitch_stand);

		yaw_stand.setup(XOR("yaw"), XOR("yaw_stnd"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("sway"), XOR("center jitter"), XOR("random"), XOR("distortion") });
		yaw_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		yaw_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_stand);

		jitter_range_stand.setup("", XOR("jitter_range_stnd"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		jitter_range_stand.AddShowCallback(callbacks::IsStandYawJitter);
		jitter_range_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&jitter_range_stand);

		rot_range_stand.setup("", XOR("rot_range_stnd"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rot_range_stand.AddShowCallback(callbacks::IsStandYawRotate);
		rot_range_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_range_stand);

		rot_speed_stand.setup("", XOR("rot_speed_stnd"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rot_speed_stand.AddShowCallback(callbacks::IsStandYawRotate);
		rot_speed_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_speed_stand);

		rand_update_stand.setup("", XOR("rand_update_stnd"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rand_update_stand.AddShowCallback(callbacks::IsStandYawRnadom);
		rand_update_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rand_update_stand);

		dir_stand.setup(XOR("direction"), XOR("dir_stnd"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_stand.AddShowCallback(callbacks::HasStandYaw);
		dir_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_stand);

		dir_time_stand.setup("", XOR("dir_time_stnd"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_time_stand.AddShowCallback(callbacks::HasStandYaw);
		dir_time_stand.AddShowCallback(callbacks::IsStandDirAuto);
		dir_time_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_time_stand);

		dir_custom_stand.setup("", XOR("dir_custom_stnd"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_custom_stand.AddShowCallback(callbacks::HasStandYaw);
		dir_custom_stand.AddShowCallback(callbacks::IsStandDirCustom);
		dir_custom_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_custom_stand);

		yaw_add_stand.setup(XOR("yaw add"), XOR("yaw_add_stand"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		yaw_add_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		yaw_add_stand.AddShowCallback(callbacks::HasStandYaw);
		yaw_add_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_add_stand);

		base_angle_stand.setup(XOR("base angle"), XOR("base_angle_stand"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		base_angle_stand.AddShowCallback(callbacks::HasStandYaw);
		base_angle_stand.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&base_angle_stand);

		dir_lock.setup(XOR("lock direction"), XOR("dir_lock"));
		dir_lock.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_lock.AddShowCallback(callbacks::HasStandYaw);
		dir_lock.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_lock);

		body_fake_stand.setup(XOR("fake body"), XOR("body_fake_stnd"));
		body_fake_stand.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&body_fake_stand);

		body_fake_angle_stand.setup("", XOR("body_fake_angle_stand"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		body_fake_angle_stand.AddShowCallback(callbacks::HasStandYaw);
		body_fake_angle_stand.AddShowCallback(callbacks::HasLbyStand);
		body_fake_angle_stand.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&body_fake_angle_stand);

		body_fake_twist.setup("twist", XOR("body_fake_twist"));
		body_fake_twist.AddShowCallback(callbacks::HasStandYaw);
		body_fake_twist.AddShowCallback(callbacks::HasLbyStand);
		body_fake_twist.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&body_fake_twist);

		disable_body_fake_fakewalk.setup("disable on fakewalk", XOR("disable_body_fake_fakewalk"));
		disable_body_fake_fakewalk.AddShowCallback(callbacks::HasStandYaw);
		disable_body_fake_fakewalk.AddShowCallback(callbacks::HasLbyStand);
		disable_body_fake_fakewalk.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&disable_body_fake_fakewalk);

		disable_body_fake_manual.setup("disable on manual", XOR("disable_body_fake_manual"));
		disable_body_fake_manual.AddShowCallback(callbacks::HasStandYaw);
		disable_body_fake_manual.AddShowCallback(callbacks::HasLbyStand);
		disable_body_fake_manual.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&disable_body_fake_manual);

		// walk.
		pitch_walk.setup(XOR("pitch"), XOR("pitch_walk"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		pitch_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&pitch_walk);

		yaw_walk.setup(XOR("yaw"), XOR("yaw_walk"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("sway"), XOR("center jitter"), XOR("random") });
		yaw_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		yaw_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_walk);

		jitter_range_walk.setup("", XOR("jitter_range_walk"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		jitter_range_walk.AddShowCallback(callbacks::IsWalkYawJitter);
		jitter_range_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&jitter_range_walk);

		rot_range_walk.setup("", XOR("rot_range_walk"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rot_range_walk.AddShowCallback(callbacks::IsWalkYawRotate);
		rot_range_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_range_walk);

		rot_speed_walk.setup("", XOR("rot_speed_walk"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rot_speed_walk.AddShowCallback(callbacks::IsWalkYawRotate);
		rot_speed_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_speed_walk);

		rand_update_walk.setup("", XOR("rand_update_walk"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rand_update_walk.AddShowCallback(callbacks::IsWalkYawRandom);
		rand_update_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rand_update_walk);

		dir_walk.setup(XOR("direction"), XOR("dir_walk"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_walk.AddShowCallback(callbacks::WalkHasYaw);
		dir_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_walk);

		dir_time_walk.setup("", XOR("dir_time_walk"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_time_walk.AddShowCallback(callbacks::WalkHasYaw);
		dir_time_walk.AddShowCallback(callbacks::IsWalkDirAuto);
		dir_time_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_time_walk);

		dir_custom_walk.setup("", XOR("dir_custom_walk"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_custom_walk.AddShowCallback(callbacks::WalkHasYaw);
		dir_custom_walk.AddShowCallback(callbacks::IsWalkDirCustom);
		dir_custom_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_custom_walk);

		yaw_add_walk.setup(XOR("yaw add"), XOR("yaw_add_walk"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		yaw_add_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		yaw_add_walk.AddShowCallback(callbacks::WalkHasYaw);
		yaw_add_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_add_walk);

		base_angle_walk.setup(XOR("base angle"), XOR("base_angle_walk"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		base_angle_walk.AddShowCallback(callbacks::WalkHasYaw);
		base_angle_walk.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&base_angle_walk);

		// air.
		pitch_air.setup(XOR("pitch"), XOR("pitch_air"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		pitch_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&pitch_air);

		yaw_air.setup(XOR("yaw"), XOR("yaw_air"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("sway"), XOR("center jitter"), XOR("random") });
		yaw_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		yaw_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_air);

		jitter_range_air.setup("", XOR("jitter_range_air"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		jitter_range_air.AddShowCallback(callbacks::IsAirYawJitter);
		jitter_range_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&jitter_range_air);

		rot_range_air.setup("", XOR("rot_range_air"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rot_range_air.AddShowCallback(callbacks::IsAirYawRotate);
		rot_range_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_range_air);

		rot_speed_air.setup("", XOR("rot_speed_air"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rot_speed_air.AddShowCallback(callbacks::IsAirYawRotate);
		rot_speed_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rot_speed_air);

		rand_update_air.setup("", XOR("rand_update_air"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rand_update_air.AddShowCallback(callbacks::IsAirYawRandom);
		rand_update_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&rand_update_air);

		dir_air.setup(XOR("direction"), XOR("dir_air"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_air.AddShowCallback(callbacks::AirHasYaw);
		dir_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_air);

		dir_time_air.setup("", XOR("dir_time_air"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_time_air.AddShowCallback(callbacks::AirHasYaw);
		dir_time_air.AddShowCallback(callbacks::IsAirDirAuto);
		dir_time_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_time_air);

		dir_custom_air.setup("", XOR("dir_custom_air"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_custom_air.AddShowCallback(callbacks::AirHasYaw);
		dir_custom_air.AddShowCallback(callbacks::IsAirDirCustom);
		dir_custom_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&dir_custom_air);

		yaw_add_air.setup(XOR("yaw add"), XOR("yaw_add_air"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		yaw_add_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		yaw_add_air.AddShowCallback(callbacks::AirHasYaw);
		yaw_add_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&yaw_add_air);

		base_angle_air.setup(XOR("base angle"), XOR("base_angle_air"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		base_angle_air.AddShowCallback(callbacks::AirHasYaw);
		base_angle_air.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&base_angle_air);

		manual_front.setup(XOR("manual front"), XOR("manual_front"));
		manual_front.SetToggleCallback(callbacks::UpdateFront);
		manual_front.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&manual_front, 1);

		manual_back.setup(XOR("manual back"), XOR("manual_back"));
		manual_back.SetToggleCallback(callbacks::UpdateBack);
		manual_back.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&manual_back, 1);

		manual_right.setup(XOR("manual right"), XOR("manual_right"));
		manual_right.SetToggleCallback(callbacks::UpdateRight);
		manual_right.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&manual_right, 1);

		manual_left.setup(XOR("manual left"), XOR("manual_left"));
		manual_left.SetToggleCallback(callbacks::UpdateLeft);
		manual_left.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&manual_left, 1);

		disable_rotation_manual.setup(XOR("static manual yaw"), XOR("disable_rotation_manual"));
		disable_rotation_manual.AddShowCallback(callbacks::IsTabGeneralAntiAim);
		RegisterElement(&disable_rotation_manual, 1);

		desync_foot_yaw.setup(XOR("desync foot yaw"), XOR("desync_foot_yaw"));
		desync_foot_yaw.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&desync_foot_yaw);

		fake_flick_mode.setup(XOR("fake flick mode"), XOR("fake_flick_mode"), { XOR("double"), XOR("single") });
		fake_flick_mode.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_flick_mode);

		fake_flick.setup(XOR("fake flick"), XOR("fake_flick"));
		fake_flick.SetToggleCallback(callbacks::UpdateFakeFlick);
		fake_flick.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_flick);

		fake_flick_range.setup("", XOR("fake_flick_range"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		fake_flick_range.AddShowCallback(callbacks::SingleFlick);
		fake_flick_range.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_flick_range);

		fake_flick_color.setup("", XOR("fake_flick_color"), colors::burgundy, nullptr, true);
		fake_flick_color.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_flick_color);

		inverter.setup("inverter", XOR("inverter"));
		inverter.SetToggleCallback(callbacks::UpdateInverter);
		RegisterElement(&inverter);

		// col2.
		fake_yaw.setup(XOR("fake yaw"), XOR("fake_yaw"), { XOR("off"), XOR("default"), XOR("relative"), XOR("jitter"), XOR("rotate"), XOR("random"), XOR("local view") });
		fake_yaw.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_yaw);

		fake_relative.setup("", XOR("fake_relative"), -90.f, 90.f, false, 0, 0.f, 5.f, XOR(L"°"));
		fake_relative.AddShowCallback(callbacks::IsFakeAntiAimRelative);
		fake_relative.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_relative);

		fake_jitter_range.setup("", XOR("fake_jitter_range"), 1.f, 90.f, false, 0, 0.f, 5.f, XOR(L"°"));
		fake_jitter_range.AddShowCallback(callbacks::IsFakeAntiAimJitter);
		fake_jitter_range.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&fake_jitter_range);

		// col 2.
		lag_enable.setup(XOR("fake-lag"), XOR("lag_enable"));
		lag_enable.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_enable, 1);

		lag_active.setup("", XOR("lag_active"), { XOR("move"), XOR("air"), XOR("crouch") }, false);
		lag_active.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_active, 1);

		lag_mode.setup("", XOR("lag_mode"), { XOR("max"), XOR("break"), XOR("random"), XOR("break step") }, false);
		lag_mode.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_mode, 1);

		lag_limit.setup(XOR("limit"), XOR("lag_limit"), 1, 15, true, 0, 1, 1.f);
		lag_limit.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_limit, 1);

		lag_disable.setup(XOR("disable lag on key"), XOR("lag_disable"));
		RegisterElement(&lag_disable, 1);

		lag_shot.setup(XOR("on shot"), XOR("lag_shot"));
		lag_shot.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_shot, 1);

		lag_peek.setup(XOR("on peek"), XOR("lag_peek"));
		lag_peek.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_peek, 1);

		lag_land.setup(XOR("on land"), XOR("lag_land"));
		lag_land.AddShowCallback(callbacks::IsTabFakeAntiAim);
		RegisterElement(&lag_land, 1);
	}
};

class PlayersTab : public Tab {
public:
	Dropdown tab;

	MultiDropdown box;
	Colorpicker   box_enemy;
	Colorpicker   box_friendly;
	Checkbox      dormant;
	Checkbox      offscreen;
	Colorpicker   offscreen_color;
	MultiDropdown name;
	Colorpicker   name_color;
	MultiDropdown health;
	MultiDropdown flags_enemy;
	MultiDropdown flags_friendly;
	MultiDropdown weapon;
	Dropdown      weapon_mode;
	Checkbox      ammo;
	Colorpicker   ammo_color;
	Checkbox      lby_update;
	Colorpicker   lby_update_color;

	// col2.
	MultiDropdown skeleton;
	Colorpicker   skeleton_enemy;
	Colorpicker   skeleton_friendly;
	Colorpicker   skeleton_local;

	MultiDropdown glow;
	Colorpicker   glow_enemy;
	Colorpicker   glow_friendly;
	Colorpicker   glow_local;
	MultiDropdown chams_enemy;
	Dropdown      chams_enemy_mat;
	Colorpicker   chams_enemy_vis;
	Colorpicker   chams_enemy_invis;
	Checkbox      chams_enemy_history;
	Colorpicker   chams_enemy_history_col;
	MultiDropdown chams_friendly;
	Dropdown      chams_friendly_mat;
	Colorpicker   chams_friendly_vis;
	Colorpicker   chams_friendly_invis;
	Checkbox      chams_local;
	Dropdown      chams_local_mat;
	Colorpicker   chams_local_col;
	Checkbox      chams_local_scope;

	MultiDropdown custom_health;
	Colorpicker   custom_health_col_enemy;
	Colorpicker   custom_health_col_friendly;

public:
	void init() {
		SetTitle(XOR("players"));

		tab.setup(XOR(""), XOR("tab"), { XOR("esp"), XOR("model") }, false);
		RegisterElement(&tab);

		box.setup(XOR("boxes"), XOR("box"), { XOR("enemy"), XOR("friendly") });
		box.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&box);

		box_enemy.setup(XOR("box enemy color"), XOR("box_enemy"), { 150, 200, 60, 255 });
		box_enemy.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&box_enemy);

		box_friendly.setup(XOR("box friendly color"), XOR("box_friendly"), { 255, 200, 0, 255 });
		box_friendly.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&box_friendly);

		dormant.setup(XOR("dormant enemies"), XOR("dormant"));
		dormant.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&dormant);

		offscreen.setup(XOR("enemy offscreen esp"), XOR("offscreen"));
		offscreen.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&offscreen);

		offscreen_color.setup(XOR("offscreen esp color"), XOR("offscreen_color"), colors::white);
		offscreen_color.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&offscreen_color);

		name.setup(XOR("name"), XOR("name"), { XOR("enemy"), XOR("friendly") });
		name.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&name);

		name_color.setup(XOR("name color"), XOR("name_color"), colors::white);
		name_color.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&name_color);

		health.setup(XOR("health"), XOR("health"), { XOR("enemy"), XOR("friendly") });
		health.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&health);

		custom_health.setup(XOR("custom health color"), XOR("custom_health"), { XOR("enemy"), XOR("friendly") });
		custom_health.AddShowCallback(callbacks::HealthBar);
		custom_health.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&custom_health);

		custom_health_col_enemy.setup(XOR("enemy health color"), XOR("custom_health_enemy"), colors::burgundy);
		custom_health_col_enemy.AddShowCallback(callbacks::HealthBar);
		custom_health_col_enemy.AddShowCallback(callbacks::CustomHealthEnemy);
		custom_health_col_enemy.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&custom_health_col_enemy);

		custom_health_col_friendly.setup(XOR("friendly health color"), XOR("custom_health_friendly"), colors::burgundy);
		custom_health_col_friendly.AddShowCallback(callbacks::HealthBar);
		custom_health_col_friendly.AddShowCallback(callbacks::CustomHealthFriendly);
		custom_health_col_friendly.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&custom_health_col_friendly);

		flags_enemy.setup(XOR("flags enemy"), XOR("flags_enemy"), { XOR("money"), XOR("armor"), XOR("scoped"), XOR("flashed"), XOR("reloading"), XOR("bomb"), XOR("resolver") });
		flags_enemy.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&flags_enemy, 1);

		flags_friendly.setup(XOR("flags friendly"), XOR("flags_friendly"), { XOR("money"), XOR("armor"), XOR("scoped"), XOR("flashed"), XOR("reloading"), XOR("bomb") });
		flags_friendly.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&flags_friendly, 1);

		weapon.setup(XOR("weapon"), XOR("weapon"), { XOR("enemy"), XOR("friendly") });
		weapon.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&weapon, 1);

		weapon_mode.setup("", XOR("weapon_mode"), { XOR("text"), XOR("icon") }, false);
		weapon_mode.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&weapon_mode, 1);

		ammo.setup(XOR("ammo"), XOR("ammo"));
		ammo.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&ammo, 1);

		ammo_color.setup(XOR("color"), XOR("ammo_color"), colors::burgundy);
		ammo_color.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&ammo_color, 1);

		lby_update.setup(XOR("lby update"), XOR("lby_update"));
		lby_update.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&lby_update, 1);

		lby_update_color.setup(XOR("color"), XOR("lby_update_color"), colors::orange);
		lby_update_color.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&lby_update_color, 1);

		// col2.
		skeleton.setup(XOR("skeleton"), XOR("skeleton"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		skeleton.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&skeleton, 1);

		skeleton_enemy.setup(XOR("enemy color"), XOR("skeleton_enemy"), { 255, 255, 255, 255 });
		skeleton_enemy.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&skeleton_enemy, 1);

		skeleton_friendly.setup(XOR("friendly color"), XOR("skeleton_friendly"), { 255, 255, 255, 255 });
		skeleton_friendly.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&skeleton_friendly, 1);

		skeleton_local.setup(XOR("local color"), XOR("skeleton_local"), { 255, 255, 255, 255 });
		skeleton_local.AddShowCallback(callbacks::IsTabEspPlayers);
		RegisterElement(&skeleton_local, 1);

		glow.setup(XOR("glow"), XOR("glow"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		glow.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&glow);

		glow_enemy.setup(XOR("enemy color"), XOR("glow_enemy"), { 255, 255, 255, 255 }, nullptr, true);
		glow_enemy.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&glow_enemy);

		glow_friendly.setup(XOR("friendly color"), XOR("glow_friendly"), { 255, 255, 255, 255 }, nullptr, true);
		glow_friendly.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&glow_friendly);

		glow_local.setup(XOR("local color"), XOR("glow_local"), { 255, 255, 255, 255 }, nullptr, true);
		glow_local.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&glow_local);

		chams_enemy.setup(XOR("chams enemy"), XOR("chams_enemy"), { XOR("visible"), XOR("invisible") });
		chams_enemy.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy);

		chams_enemy_mat.setup(XOR("chams material"), XOR("chams_enemy_mat"), { XOR("normal"), XOR("flat"), XOR("metallic") });
		chams_enemy_mat.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy_mat);

		chams_enemy_vis.setup(XOR("color visible"), XOR("chams_enemy_vis"), { 255, 255, 255, 255 }, nullptr, true);
		chams_enemy_vis.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy_vis);

		chams_enemy_invis.setup(XOR("color invisible"), XOR("chams_enemy_invis"), { 255, 255, 255, 255 }, nullptr, true);
		chams_enemy_invis.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy_invis);

		chams_enemy_history.setup(XOR("chams history"), XOR("chams_history"));
		chams_enemy_history.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy_history);

		chams_enemy_history_col.setup(XOR("color"), XOR("chams_history_col"), { 255, 255, 255, 255 }, nullptr, true);
		chams_enemy_history_col.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_enemy_history_col);

		chams_friendly.setup(XOR("chams friendly"), XOR("chams_friendly"), { XOR("visible"), XOR("invisible") });
		chams_friendly.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_friendly, 1);

		chams_friendly_mat.setup(XOR("chams friendly material"), XOR("chams_friendly_mat"), { XOR("normal"), XOR("flat"), XOR("metallic") });
		chams_friendly_mat.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_friendly_mat, 1);

		chams_friendly_vis.setup(XOR("color visible"), XOR("chams_friendly_vis"), { 255, 255, 255, 255 }, nullptr, true);
		chams_friendly_vis.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_friendly_vis, 1);

		chams_friendly_invis.setup(XOR("color invisible"), XOR("chams_friendly_invis"), { 255, 255, 255, 255 }, nullptr, true);
		chams_friendly_invis.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_friendly_invis, 1);

		chams_local.setup(XOR("chams local"), XOR("chams_local"));
		chams_local.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_local, 1);

		chams_local_mat.setup(XOR("chams local material"), XOR("chams_local_mat"), { XOR("normal"), XOR("flat"), XOR("metallic") });
		chams_local_mat.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_local_mat, 1);

		chams_local_col.setup(XOR("color"), XOR("chams_local_col"), { 255, 255, 255, 255 }, nullptr, true);
		chams_local_col.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_local_col, 1);

		chams_local_scope.setup(XOR("blend when scoped"), XOR("chams_local_scope"));
		chams_local_scope.AddShowCallback(callbacks::IsTabModelPlayers);
		RegisterElement(&chams_local_scope, 1);
	}
};

class VisualsTab : public Tab {
public:
	Dropdown      tab;
	Checkbox      items;
	Checkbox      ammo;
	Colorpicker   item_color;
	Colorpicker   impact_color;
	Checkbox      proj;
	Colorpicker   proj_color;
	MultiDropdown proj_range;
	Colorpicker   proj_range_color;
	MultiDropdown planted_c4;
	Checkbox      disableteam;
	Checkbox      enemy_radar;

	// col2.
	Checkbox      novisrecoil;
	Checkbox      remove_shadows;
	Checkbox      nosmoke;
	Checkbox      nofog;
	Checkbox      noflash;
	Checkbox      noscope;
	Checkbox      nopostprocessing;
	Checkbox      fov;
	Slider        fov_amt;
	Checkbox      fov_scoped;
	Checkbox      viewmodel_fov;
	Slider        viewmodel_fov_amt;
	Slider        viewmodel_x;
	Slider        viewmodel_y;
	Slider        viewmodel_z;
	Checkbox      spectators;
	Checkbox      force_xhair;
	Checkbox      spread_xhair;
	Colorpicker   spread_xhair_col;
	Slider        spread_xhair_blend;
	Checkbox      pen_crosshair;
	MultiDropdown indicators;
	Checkbox      impacts;
	Checkbox      tracers;
	Checkbox      impact_beams;
	Colorpicker   impact_beams_color;
	Colorpicker   impact_beams_hurt_color;
	Slider        impact_beams_time;
	Keybind       thirdperson;
	Slider        thirdperson_distance;

	Checkbox	  world_modulation;
	Colorpicker   worldcolor;
	Slider        worlddarkness;
	Checkbox      fullbright;
	Slider        aspect_ratio;
	//	  world_color;
	Checkbox      transparent_props;
	Slider		  transparent_props_amount;

	Dropdown      manual_antiaim;
	Colorpicker   manual_antiaim_color;
	Colorpicker   manual_antiaim_color1;
	Colorpicker   manual_antiaim_color2;
	Colorpicker   manual_antiaim_color3;

	Keybind       manual_antiaim_switch;

public:
	void init() {
		SetTitle(XOR("visuals"));

		tab.setup(XOR(""), XOR("tab"), { XOR("general"), XOR("removals"), XOR("view") }, false);
		RegisterElement(&tab);

		items.setup(XOR("dropped weapons"), XOR("items"));
		items.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&items);

		ammo.setup(XOR("dropped weapons ammo"), XOR("ammo"));
		ammo.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&ammo);

		item_color.setup(XOR("color"), XOR("item_color"), colors::white);
		item_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&item_color);

		proj.setup(XOR("projectiles"), XOR("proj"));
		proj.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&proj);

		proj_color.setup(XOR("color"), XOR("proj_color"), colors::white);
		proj_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&proj_color);

		proj_range.setup(XOR("projectile range"), XOR("proj_range"), { XOR("frag"), XOR("molly") });
		proj_range.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&proj_range);

		proj_range_color.setup(XOR("color"), XOR("proj_range_color"), colors::burgundy);
		proj_range_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&proj_range_color);


		planted_c4.setup(XOR("planted c4"), XOR("planted_c4"), { XOR("on screen (2D)"), XOR("on bomb (3D)") });
		planted_c4.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&planted_c4);

		aspect_ratio.setup(XOR("aspect ratio"), XOR("aspect_ratio"), 0, 100, true, 0, 0, 1.f, XOR(L""));
		aspect_ratio.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&aspect_ratio);

		world_modulation.setup(XOR("world modulation"), XOR("world"));
		world_modulation.SetCallback(Visuals::ModulateWorld);
		world_modulation.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&world_modulation);

		worldcolor.setup(XOR(""), XOR("world_color"), colors::white);
		worldcolor.SetCallback(Visuals::ModulateWorld);
		worldcolor.AddShowCallback(callbacks::IsNightMode);
		worldcolor.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&worldcolor);

		worlddarkness.setup("", XOR("world_darkness"), 0.f, 100.f, false, 0, 30.f, 1.f, XOR(L"%"));
		worlddarkness.SetCallback(Visuals::ModulateWorld);
		worlddarkness.AddShowCallback(callbacks::IsNightMode);
		worlddarkness.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&worlddarkness);

		fullbright.setup(XOR("full-bright"), XOR("fullbright"));
		fullbright.SetCallback(Visuals::ModulateWorld);
		fullbright.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&fullbright);

		transparent_props.setup(XOR("props"), XOR("transparent_props"));
		transparent_props.SetCallback(Visuals::ModulateWorld);
		transparent_props.AddShowCallback(callbacks::IsNightMode);
		transparent_props.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&transparent_props);

		transparent_props_amount.setup("", XOR("transparent_props_amount"), 0.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		transparent_props_amount.SetCallback(Visuals::ModulateWorld);
		transparent_props_amount.AddShowCallback(callbacks::IsNightMode);
		transparent_props_amount.AddShowCallback(callbacks::IsTransparentProps);
		transparent_props_amount.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&transparent_props_amount);

		enemy_radar.setup(XOR("force enemies on radar"), XOR("enemy_radar"));
		enemy_radar.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&enemy_radar);

		viewmodel_fov.setup(XOR("override viewmodel fov"), XOR("viewmodel_fov"));
		viewmodel_fov.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&viewmodel_fov);

		viewmodel_fov_amt.setup("", XOR("viewmodel_fov_amt"), 60.f, 140.f, false, 0, 90.f, 1.f, XOR(L"°"));
		viewmodel_fov_amt.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&viewmodel_fov_amt);

		viewmodel_x.setup("x", XOR("viewmodel_x"), -25, 25, true, 0, 5.f, 1.f);
		viewmodel_x.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&viewmodel_x);
		viewmodel_y.setup("y", XOR("viewmodel_y"), -25, 25, true, 0, 0.f, 1.f);
		viewmodel_y.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&viewmodel_y);
		viewmodel_z.setup("z", XOR("viewmodel_z"), -25, 25, true, 0, -3.f, 1.f);
		viewmodel_z.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&viewmodel_z);

		// col2.
		disableteam.setup(XOR("remove teammates render"), XOR("disableteam"));
		disableteam.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&disableteam);


		remove_shadows.setup(XOR("remove visual shadows"), XOR("remove_shadows"));
		remove_shadows.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&remove_shadows);

		novisrecoil.setup(XOR("remove visual recoil"), XOR("novisrecoil"));
		novisrecoil.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&novisrecoil);

		nosmoke.setup(XOR("remove smoke grenades"), XOR("nosmoke"));
		nosmoke.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&nosmoke);

		nofog.setup(XOR("remove fog"), XOR("nofog"));
		nofog.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&nofog);

		noflash.setup(XOR("remove flashbangs"), XOR("noflash"));
		noflash.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&noflash);

		noscope.setup(XOR("remove scope"), XOR("noscope"));
		noscope.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&noscope);

		nopostprocessing.setup(XOR("remove post processing"), XOR("nopostprocessing"));
		nopostprocessing.AddShowCallback(callbacks::IsTabRemovalsVisuals);
		RegisterElement(&nopostprocessing);

		fov.setup(XOR("override fov"), XOR("fov"));
		fov.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&fov);

		fov_amt.setup("", XOR("fov_amt"), 60.f, 140.f, false, 0, 90.f, 1.f, XOR(L"°"));
		fov_amt.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&fov_amt);

		fov_scoped.setup(XOR("override fov when scoped"), XOR("fov_scoped"));
		fov_scoped.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&fov_scoped);

		spectators.setup(XOR("show spectator list"), XOR("spectators"));
		spectators.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&spectators);

		force_xhair.setup(XOR("force crosshair"), XOR("force_xhair"));
		force_xhair.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&force_xhair);

		spread_xhair.setup(XOR("visualize spread"), XOR("spread_xhair"));
		spread_xhair.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&spread_xhair);

		spread_xhair_col.setup(XOR("visualize spread color"), XOR("spread_xhair_col"), colors::burgundy);
		spread_xhair_col.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&spread_xhair_col);

		spread_xhair_blend.setup("", XOR("spread_xhair_blend"), 0.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		spread_xhair_blend.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&spread_xhair_blend);


		indicators.setup(XOR("indicators"), XOR("indicators"), { XOR("lby"), XOR("lag compensation"), XOR("fake latency"), XOR("damage override"), XOR("force baim"), XOR("headglitch aimbot"), XOR("peek")});
		indicators.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&indicators, 1);

		//crosshair_ind.setup(XOR("crosshair indicators"), XOR("crosshair_ind"));
		//crosshair_ind.AddShowCallback(callbacks::IsTabGeneralVisuals);
		//RegisterElement(&crosshair_ind, 1);

		manual_antiaim.setup(XOR("manual antiaim arrows"), XOR("manual_antiaim"), { XOR("none"), XOR("arrows"), XOR("default") });
		manual_antiaim.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&manual_antiaim, 1);

		manual_antiaim_color.setup(XOR(""), XOR("manual_antiaim_color"), colors::burgundy, nullptr, true);
		manual_antiaim_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&manual_antiaim_color, 1);

		//manual_antiaim_color1.setup(XOR(""), XOR("manual_antiaim_color1"), colors::burgundy, nullptr, true);
		//manual_antiaim_color1.AddShowCallback(callbacks::IsTabGeneralVisuals);
		//RegisterElement(&manual_antiaim_color1, 1);

		//manual_antiaim_color2.setup(XOR(""), XOR("manual_antiaim_color2"), colors::burgundy, nullptr, true);
		//manual_antiaim_color2.AddShowCallback(callbacks::IsTabGeneralVisuals);
		//RegisterElement(&manual_antiaim_color2, 1);

		//manual_antiaim_color3.setup(XOR(""), XOR("manual_antiaim_color3"), colors::burgundy, nullptr, true);
		//manual_antiaim_color3.AddShowCallback(callbacks::IsTabGeneralVisuals);
		//RegisterElement(&manual_antiaim_color3, 1);

		//manual_antiaim_switch.setup(XOR("switch manual"), XOR("manual_antiaim_switch"));
		//manual_antiaim_switch.AddShowCallback(callbacks::IsTabGeneralVisuals);
		//manual_antiaim_switch.SetToggleCallback(callbacks::SwitchManualMode);
		//RegisterElement(&manual_antiaim_switch, 1);

		tracers.setup(XOR("grenade simulation"), XOR("tracers"));
		tracers.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&tracers, 1);

		pen_crosshair.setup(XOR("penetration crosshair"), XOR("pen_xhair"));
		pen_crosshair.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&pen_crosshair, 1);

		impacts.setup(XOR("bullet impacts"), XOR("impacts"));
		impacts.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&impacts,1);

		impact_beams.setup(XOR("impact beams"), XOR("impact_beams"));
		impact_beams.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&impact_beams, 1);

		impact_beams_color.setup(XOR("impact beams color"), XOR("impact_beams_color"), colors::light_blue);
		impact_beams_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&impact_beams_color, 1);

		impact_beams_hurt_color.setup(XOR("impact beams hurt color"), XOR("impact_beams_hurt_color"), colors::red);
		impact_beams_hurt_color.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&impact_beams_hurt_color, 1);

		impact_beams_time.setup(XOR("impact beams time"), XOR("impact_beams_time"), 1.f, 10.f, true, 0, 1.f, 1.f, XOR(L"s"));
		impact_beams_time.AddShowCallback(callbacks::IsTabGeneralVisuals);
		RegisterElement(&impact_beams_time, 1);

		thirdperson.setup(XOR("thirdperson"), XOR("thirdperson"));
		thirdperson.SetToggleCallback(callbacks::ToggleThirdPerson);
		thirdperson.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&thirdperson, 1);

		thirdperson_distance.setup("", XOR("thirdperson_distance"), 60.f, 150.f, false, 0, 120.f, 1.f);
		thirdperson_distance.AddShowCallback(callbacks::IsTabViewVisuals);
		RegisterElement(&thirdperson_distance, 1);
	}
};

class MovementTab : public Tab {
public:
	Checkbox bhop;
	Checkbox airduck;
	Checkbox autostrafe;
	Checkbox quickstop;

	Dropdown slidewalk;
	Keybind  fakewalk;
	Slider   fakewalk_speed;
	Keybind  autopeek;
	Colorpicker  autopeek_color;

public:
	void init() {
		SetTitle(XOR("movement"));

		bhop.setup(XOR("automatic jump"), XOR("bhop"));
		RegisterElement(&bhop);

		airduck.setup(XOR("duck in air"), XOR("airduck"));
		RegisterElement(&airduck);

		autostrafe.setup(XOR("automatic strafe"), XOR("autostrafe"));
		RegisterElement(&autostrafe);

		slidewalk.setup(XOR("slide walk"), XOR("slidewalk"), { XOR("prevent"), XOR("force"), XOR("jitter") });
		RegisterElement(&slidewalk);

		quickstop.setup(XOR("quick stop"), XOR("quickstop"));
		RegisterElement(&quickstop);

		fakewalk.setup(XOR("fake-walk"), XOR("fakewalk"));
		RegisterElement(&fakewalk, 1);

		fakewalk_speed.setup(XOR("fake-walk speed"), XOR("fakewalk_speed"), 1, 15, true, 1, 1, 1, XOR(L"t"));
		RegisterElement(&fakewalk_speed, 1);

		autopeek.setup(XOR("automatic peek"), XOR("autopeek"));
		RegisterElement(&autopeek, 1);

		autopeek_color.setup(XOR(""), XOR("autopeek_color"), colors::burgundy, nullptr, true);
		RegisterElement(&autopeek_color, 1);
	}
};

class SkinsTab : public Tab {
public:
	Checkbox enable;
	Checkbox custom_paintkit;
	Checkbox stickers;
	Slider stickers1;
	Slider stickers2;
	Slider stickers3;
	Slider stickers4;


	Edit2     id_deagle;
	Checkbox stattrak_deagle;
	Slider   quality_deagle;
	Slider	 seed_deagle;

	Colorpicker    deagle_color;
	Colorpicker    deagle_color1;
	Colorpicker  deagle_color2;
	Colorpicker   deagle_color3;


	Edit2     id_elite;
	Checkbox stattrak_elite;
	Slider   quality_elite;
	Slider	 seed_elite;

	Edit2     id_fiveseven;
	Checkbox stattrak_fiveseven;
	Slider   quality_fiveseven;
	Slider	 seed_fiveseven;

	Colorpicker    fiveseven_color;
	Colorpicker    fiveseven_color1;
	Colorpicker   fiveseven_color2;
	Colorpicker   fiveseven_color3;

	Edit2     id_glock;
	Checkbox stattrak_glock;
	Slider   quality_glock;
	Slider	 seed_glock;

	Colorpicker    glock_color;
	Colorpicker    glock_color1;
	Colorpicker   glock_color2;
	Colorpicker   glock_color3;

	Edit2     id_ak47;
	Checkbox stattrak_ak47;
	Slider   quality_ak47;
	Slider	 seed_ak47;

	Edit2     id_aug;
	Checkbox stattrak_aug;
	Slider   quality_aug;
	Slider	 seed_aug;

	Edit2     id_awp;
	Checkbox stattrak_awp;
	Slider   quality_awp;
	Slider	 seed_awp;
	Colorpicker    awp_color;
	Colorpicker    awp_color1;
	Colorpicker    awp_color2;
	Colorpicker    awp_color3;

	Edit2     id_famas;
	Checkbox stattrak_famas;
	Slider   quality_famas;
	Slider	 seed_famas;

	Edit2     id_g3sg1;
	Checkbox stattrak_g3sg1;
	Slider   quality_g3sg1;
	Slider	 seed_g3sg1;
	Colorpicker    g3sg1_color;
	Colorpicker    g3sg1_color1;
	Colorpicker    g3sg1_color2;
	Colorpicker    g3sg1_color3;

	Edit2     id_galil;
	Checkbox stattrak_galil;
	Slider   quality_galil;
	Slider	 seed_galil;

	Edit2     id_m249;
	Checkbox stattrak_m249;
	Slider   quality_m249;
	Slider	 seed_m249;

	Edit2     id_m4a4;
	Checkbox stattrak_m4a4;
	Slider   quality_m4a4;
	Slider	 seed_m4a4;

	Edit2     id_mac10;
	Checkbox stattrak_mac10;
	Slider   quality_mac10;
	Slider	 seed_mac10;

	Edit2     id_p90;
	Checkbox stattrak_p90;
	Slider   quality_p90;
	Slider	 seed_p90;

	Edit2     id_ump45;
	Checkbox stattrak_ump45;
	Slider   quality_ump45;
	Slider	 seed_ump45;

	Edit2     id_xm1014;
	Checkbox stattrak_xm1014;
	Slider   quality_xm1014;
	Slider	 seed_xm1014;

	Edit2     id_bizon;
	Checkbox stattrak_bizon;
	Slider   quality_bizon;
	Slider	 seed_bizon;

	Edit2     id_mag7;
	Checkbox stattrak_mag7;
	Slider   quality_mag7;
	Slider	 seed_mag7;

	Colorpicker    mag7_color;
	Colorpicker    mag7_color1;
	Colorpicker mag7_color2;
	Colorpicker  mag7_color3;

	Edit2     id_negev;
	Checkbox stattrak_negev;
	Slider   quality_negev;
	Slider	 seed_negev;

	Edit2     id_sawedoff;
	Checkbox stattrak_sawedoff;
	Slider   quality_sawedoff;
	Slider	 seed_sawedoff;

	Edit2     id_tec9;
	Checkbox stattrak_tec9;
	Slider   quality_tec9;
	Slider	 seed_tec9;
	Colorpicker    tec9_color;
	Colorpicker    tec9_color1;
	Colorpicker tec9_color2;
	Colorpicker   tec9_color3;

	Edit2     id_p2000;
	Checkbox stattrak_p2000;
	Slider   quality_p2000;
	Slider	 seed_p2000;
	Colorpicker    p2000_color;
	Colorpicker     p2000_color1;
	Colorpicker  p2000_color2;
	Colorpicker   p2000_color3;

	Edit2     id_mp7;
	Checkbox stattrak_mp7;
	Slider   quality_mp7;
	Slider	 seed_mp7;

	Edit2     id_mp9;
	Checkbox stattrak_mp9;
	Slider   quality_mp9;
	Slider	 seed_mp9;

	Edit2     id_nova;
	Checkbox stattrak_nova;
	Slider   quality_nova;
	Slider	 seed_nova;

	Edit2     id_p250;
	Checkbox stattrak_p250;
	Slider   quality_p250;
	Slider	 seed_p250;

	Edit2     id_scar20;
	Checkbox stattrak_scar20;
	Slider   quality_scar20;
	Slider	 seed_scar20;
	Colorpicker    scar20_color;
	Colorpicker    scar20_color1;
	Colorpicker    scar20_color2;
	Colorpicker    scar20_color3;

	Edit2     id_sg553;
	Checkbox stattrak_sg553;
	Slider   quality_sg553;
	Slider	 seed_sg553;

	Edit2     id_ssg08;
	Checkbox stattrak_ssg08;
	Slider   quality_ssg08;
	Slider	 seed_ssg08;
	Colorpicker    ssg08_color;
	Colorpicker    ssg08_color1;
	Colorpicker    ssg08_color2;
	Colorpicker    ssg08_color3;

	Edit2     id_m4a1s;
	Checkbox stattrak_m4a1s;
	Slider   quality_m4a1s;
	Slider	 seed_m4a1s;

	Edit2     id_usps;
	Checkbox stattrak_usps;
	Slider   quality_usps;
	Slider	 seed_usps;

	Edit2     id_cz75a;
	Checkbox stattrak_cz75a;
	Slider   quality_cz75a;
	Slider	 seed_cz75a;

	Edit2     id_revolver;
	Checkbox stattrak_revolver;
	Slider   quality_revolver;
	Slider	 seed_revolver;

	Edit2     id_bayonet;
	Checkbox stattrak_bayonet;
	Slider   quality_bayonet;
	Slider	 seed_bayonet;

	Edit2     id_flip;
	Checkbox stattrak_flip;
	Slider   quality_flip;
	Slider	 seed_flip;

	Edit2     id_gut;
	Checkbox stattrak_gut;
	Slider   quality_gut;
	Slider	 seed_gut;

	Edit2     id_karambit;
	Checkbox stattrak_karambit;
	Slider   quality_karambit;
	Slider	 seed_karambit;

	Edit2     id_m9;
	Checkbox stattrak_m9;
	Slider   quality_m9;
	Slider	 seed_m9;

	Edit2     id_huntsman;
	Checkbox stattrak_huntsman;
	Slider   quality_huntsman;
	Slider	 seed_huntsman;

	Edit2     id_falchion;
	Checkbox stattrak_falchion;
	Slider   quality_falchion;
	Slider	 seed_falchion;

	Edit2     id_bowie;
	Checkbox stattrak_bowie;
	Slider   quality_bowie;
	Slider	 seed_bowie;

	Edit2     id_butterfly;
	Checkbox stattrak_butterfly;
	Slider   quality_butterfly;
	Slider	 seed_butterfly;

	Edit2     id_daggers;
	Checkbox stattrak_daggers;
	Slider   quality_daggers;
	Slider	 seed_daggers;

	// col 2.
	Dropdown knife;
	Dropdown glove;
	Edit2	 glove_id;

public:
	void init() {
		SetTitle(XOR("skins"));

		enable.setup(XOR("enable"), XOR("skins_enable"));
		enable.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&enable);

		custom_paintkit.setup(XOR("paint-kit"), XOR("custom_paintkit"));
		custom_paintkit.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&custom_paintkit);

		// weapons...
		id_deagle.setup(XOR("paintkit id"), XOR("id_deagle"), 3);
		id_deagle.SetCallback(callbacks::SkinUpdate);
		id_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&id_deagle);

		stattrak_deagle.setup(XOR("stattrak"), XOR("stattrak_deagle"));
		stattrak_deagle.SetCallback(callbacks::SkinUpdate);
		stattrak_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&stattrak_deagle);

		quality_deagle.setup(XOR("quality"), XOR("quality_deagle"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_deagle.SetCallback(callbacks::SkinUpdate);
		quality_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&quality_deagle);

		seed_deagle.setup(XOR("seed"), XOR("seed_deagle"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_deagle.SetCallback(callbacks::SkinUpdate);
		seed_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&seed_deagle);

		id_elite.setup(XOR("paintkit id"), XOR("id_elite"), 3);
		id_elite.SetCallback(callbacks::SkinUpdate);
		id_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&id_elite);

		stattrak_elite.setup(XOR("stattrak"), XOR("stattrak_elite"));
		stattrak_elite.SetCallback(callbacks::SkinUpdate);
		stattrak_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&stattrak_elite);

		quality_elite.setup(XOR("quality"), XOR("quality_elite"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_elite.SetCallback(callbacks::SkinUpdate);
		quality_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&quality_elite);

		seed_elite.setup(XOR("seed"), XOR("seed_elite"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_elite.SetCallback(callbacks::SkinUpdate);
		seed_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&seed_elite);

		id_fiveseven.setup(XOR("paintkit id"), XOR("id_fiveseven"), 3);
		id_fiveseven.SetCallback(callbacks::SkinUpdate);
		id_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&id_fiveseven);

		stattrak_fiveseven.setup(XOR("stattrak"), XOR("stattrak_fiveseven"));
		stattrak_fiveseven.SetCallback(callbacks::SkinUpdate);
		stattrak_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&stattrak_fiveseven);

		quality_fiveseven.setup(XOR("quality"), XOR("quality_fiveseven"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_fiveseven.SetCallback(callbacks::SkinUpdate);
		quality_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&quality_fiveseven);

		seed_fiveseven.setup(XOR("seed"), XOR("seed_fiveseven"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_fiveseven.SetCallback(callbacks::SkinUpdate);
		seed_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&seed_fiveseven);

		fiveseven_color.setup(XOR("color"), XOR("fiveseven_color"), colors::white);
		fiveseven_color.SetCallback(callbacks::SkinUpdate);
		fiveseven_color.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&fiveseven_color);

		fiveseven_color1.setup(XOR("color 1"), XOR("fiveseven_color1"), colors::white);
		fiveseven_color1.SetCallback(callbacks::SkinUpdate);
		fiveseven_color1.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&fiveseven_color1);

		fiveseven_color2.setup(XOR("color 2"), XOR("fiveseven_color2"), colors::white);
		fiveseven_color2.SetCallback(callbacks::SkinUpdate);
		fiveseven_color2.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&fiveseven_color2);

		fiveseven_color3.setup(XOR("color 3"), XOR("fiveseven_color3"), colors::white);
		fiveseven_color3.SetCallback(callbacks::SkinUpdate);
		fiveseven_color3.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&fiveseven_color3);

		id_glock.setup(XOR("paintkit id"), XOR("id_glock"), 3);
		id_glock.SetCallback(callbacks::SkinUpdate);
		id_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&id_glock);

		stattrak_glock.setup(XOR("stattrak"), XOR("stattrak_glock"));
		stattrak_glock.SetCallback(callbacks::SkinUpdate);
		stattrak_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&stattrak_glock);

		quality_glock.setup(XOR("quality"), XOR("quality_glock"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_glock.SetCallback(callbacks::SkinUpdate);
		quality_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&quality_glock);

		seed_glock.setup(XOR("seed"), XOR("seed_glock"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_glock.SetCallback(callbacks::SkinUpdate);
		seed_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&seed_glock);

		glock_color.setup(XOR("color"), XOR("glock_color"), colors::white);
		glock_color.SetCallback(callbacks::SkinUpdate);
		glock_color.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&glock_color);

		glock_color1.setup(XOR("color 1"), XOR("glock_color1"), colors::white);
		glock_color1.SetCallback(callbacks::SkinUpdate);
		glock_color1.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&glock_color1);

		glock_color2.setup(XOR("color 2"), XOR("glock_color2"), colors::white);
		glock_color2.SetCallback(callbacks::SkinUpdate);
		glock_color2.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&glock_color2);

		glock_color3.setup(XOR("color 3"), XOR("glock_color3"), colors::white);
		glock_color3.SetCallback(callbacks::SkinUpdate);
		glock_color3.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&glock_color3);

		id_ak47.setup(XOR("paintkit id"), XOR("id_ak47"), 3);
		id_ak47.SetCallback(callbacks::SkinUpdate);
		id_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&id_ak47);

		stattrak_ak47.setup(XOR("stattrak"), XOR("stattrak_ak47"));
		stattrak_ak47.SetCallback(callbacks::SkinUpdate);
		stattrak_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&stattrak_ak47);

		quality_ak47.setup(XOR("quality"), XOR("quality_ak47"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ak47.SetCallback(callbacks::SkinUpdate);
		quality_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&quality_ak47);

		seed_ak47.setup(XOR("seed"), XOR("seed_ak47"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ak47.SetCallback(callbacks::SkinUpdate);
		seed_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&seed_ak47);

		id_aug.setup(XOR("paintkit id"), XOR("id_aug"), 3);
		id_aug.SetCallback(callbacks::SkinUpdate);
		id_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&id_aug);

		stattrak_aug.setup(XOR("stattrak"), XOR("stattrak_aug"));
		stattrak_aug.SetCallback(callbacks::SkinUpdate);
		stattrak_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&stattrak_aug);

		quality_aug.setup(XOR("quality"), XOR("quality_aug"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_aug.SetCallback(callbacks::SkinUpdate);
		quality_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&quality_aug);

		seed_aug.setup(XOR("seed"), XOR("seed_aug"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_aug.SetCallback(callbacks::SkinUpdate);
		seed_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&seed_aug);

		id_awp.setup(XOR("paintkit id"), XOR("id_awp"), 3);
		id_awp.SetCallback(callbacks::SkinUpdate);
		id_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&id_awp);

		stattrak_awp.setup(XOR("stattrak"), XOR("stattrak_awp"));
		stattrak_awp.SetCallback(callbacks::SkinUpdate);
		stattrak_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&stattrak_awp);

		quality_awp.setup(XOR("quality"), XOR("quality_awp"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_awp.SetCallback(callbacks::SkinUpdate);
		quality_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&quality_awp);

		seed_awp.setup(XOR("seed"), XOR("seed_awp"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_awp.SetCallback(callbacks::SkinUpdate);
		seed_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&seed_awp);

		awp_color.setup(XOR("color"), XOR("awp_color"), colors::white);
		awp_color.SetCallback(callbacks::SkinUpdate);
		awp_color.AddShowCallback(callbacks::AWP);
		RegisterElement(&awp_color);

		awp_color1.setup(XOR("color 1"), XOR("awp_color1"), colors::white);
		awp_color1.SetCallback(callbacks::SkinUpdate);
		awp_color1.AddShowCallback(callbacks::AWP);
		RegisterElement(&awp_color1);

		awp_color2.setup(XOR("color 2"), XOR("awp_color2"), colors::white);
		awp_color2.SetCallback(callbacks::SkinUpdate);
		awp_color2.AddShowCallback(callbacks::AWP);
		RegisterElement(&awp_color2);

		awp_color3.setup(XOR("color 3"), XOR("awp_color3"), colors::white);
		awp_color3.SetCallback(callbacks::SkinUpdate);
		awp_color3.AddShowCallback(callbacks::AWP);
		RegisterElement(&awp_color3);

		id_famas.setup(XOR("paintkit id"), XOR("id_famas"), 3);
		id_famas.SetCallback(callbacks::SkinUpdate);
		id_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&id_famas);

		stattrak_famas.setup(XOR("stattrak"), XOR("stattrak_famas"));
		stattrak_famas.SetCallback(callbacks::SkinUpdate);
		stattrak_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&stattrak_famas);

		quality_famas.setup(XOR("quality"), XOR("quality_famas"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_famas.SetCallback(callbacks::SkinUpdate);
		quality_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&quality_famas);

		seed_famas.setup(XOR("seed"), XOR("seed_famas"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_famas.SetCallback(callbacks::SkinUpdate);
		seed_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&seed_famas);

		id_g3sg1.setup(XOR("paintkit id"), XOR("id_g3sg1"), 3);
		id_g3sg1.SetCallback(callbacks::SkinUpdate);
		id_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&id_g3sg1);

		stattrak_g3sg1.setup(XOR("stattrak"), XOR("stattrak_g3sg1"));
		stattrak_g3sg1.SetCallback(callbacks::SkinUpdate);
		stattrak_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&stattrak_g3sg1);

		quality_g3sg1.setup(XOR("quality"), XOR("quality_g3sg1"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_g3sg1.SetCallback(callbacks::SkinUpdate);
		quality_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&quality_g3sg1);

		seed_g3sg1.setup(XOR("seed"), XOR("seed_g3sg1"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_g3sg1.SetCallback(callbacks::SkinUpdate);
		seed_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&seed_g3sg1);

		g3sg1_color.setup(XOR("color"), XOR("g3sg1_color"), colors::white);
		g3sg1_color.SetCallback(callbacks::SkinUpdate);
		g3sg1_color.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&g3sg1_color);

		g3sg1_color1.setup(XOR("color 1"), XOR("g3sg1_color1"), colors::white);
		g3sg1_color1.SetCallback(callbacks::SkinUpdate);
		g3sg1_color1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&g3sg1_color1);

		g3sg1_color2.setup(XOR("color 2"), XOR("g3sg1_color2"), colors::white);
		g3sg1_color2.SetCallback(callbacks::SkinUpdate);
		g3sg1_color2.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&g3sg1_color2);

		g3sg1_color3.setup(XOR("color 3"), XOR("g3sg1_color3"), colors::white);
		g3sg1_color3.SetCallback(callbacks::SkinUpdate);
		g3sg1_color3.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&g3sg1_color3);

		id_galil.setup(XOR("paintkit id"), XOR("id_galil"), 3);
		id_galil.SetCallback(callbacks::SkinUpdate);
		id_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&id_galil);

		stattrak_galil.setup(XOR("stattrak"), XOR("stattrak_galil"));
		stattrak_galil.SetCallback(callbacks::SkinUpdate);
		stattrak_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&stattrak_galil);

		quality_galil.setup(XOR("quality"), XOR("quality_galil"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_galil.SetCallback(callbacks::SkinUpdate);
		quality_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&quality_galil);

		seed_galil.setup(XOR("seed"), XOR("seed_galil"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_galil.SetCallback(callbacks::SkinUpdate);
		seed_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&seed_galil);

		id_m249.setup(XOR("paintkit id"), XOR("id_m249"), 3);
		id_m249.SetCallback(callbacks::SkinUpdate);
		id_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&id_m249);

		stattrak_m249.setup(XOR("stattrak"), XOR("stattrak_m249"));
		stattrak_m249.SetCallback(callbacks::SkinUpdate);
		stattrak_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&stattrak_m249);

		quality_m249.setup(XOR("quality"), XOR("quality_m249"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m249.SetCallback(callbacks::SkinUpdate);
		quality_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&quality_m249);

		seed_m249.setup(XOR("seed"), XOR("seed_m249"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m249.SetCallback(callbacks::SkinUpdate);
		seed_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&seed_m249);

		id_m4a4.setup(XOR("paintkit id"), XOR("id_m4a4"), 3);
		id_m4a4.SetCallback(callbacks::SkinUpdate);
		id_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&id_m4a4);

		stattrak_m4a4.setup(XOR("stattrak"), XOR("stattrak_m4a4"));
		stattrak_m4a4.SetCallback(callbacks::SkinUpdate);
		stattrak_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&stattrak_m4a4);

		quality_m4a4.setup(XOR("quality"), XOR("quality_m4a4"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m4a4.SetCallback(callbacks::SkinUpdate);
		quality_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&quality_m4a4);

		seed_m4a4.setup(XOR("seed"), XOR("seed_m4a4"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m4a4.SetCallback(callbacks::SkinUpdate);
		seed_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&seed_m4a4);

		id_mac10.setup(XOR("paintkit id"), XOR("id_mac10"), 3);
		id_mac10.SetCallback(callbacks::SkinUpdate);
		id_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&id_mac10);

		stattrak_mac10.setup(XOR("stattrak"), XOR("stattrak_mac10"));
		stattrak_mac10.SetCallback(callbacks::SkinUpdate);
		stattrak_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&stattrak_mac10);

		quality_mac10.setup(XOR("quality"), XOR("quality_mac10"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mac10.SetCallback(callbacks::SkinUpdate);
		quality_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&quality_mac10);

		seed_mac10.setup(XOR("seed"), XOR("seed_mac10"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mac10.SetCallback(callbacks::SkinUpdate);
		seed_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&seed_mac10);

		id_p90.setup(XOR("paintkit id"), XOR("id_p90"), 3);
		id_p90.SetCallback(callbacks::SkinUpdate);
		id_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&id_p90);

		stattrak_p90.setup(XOR("stattrak"), XOR("stattrak_p90"));
		stattrak_p90.SetCallback(callbacks::SkinUpdate);
		stattrak_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&stattrak_p90);

		quality_p90.setup(XOR("quality"), XOR("quality_p90"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p90.SetCallback(callbacks::SkinUpdate);
		quality_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&quality_p90);

		seed_p90.setup(XOR("seed"), XOR("seed_p90"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p90.SetCallback(callbacks::SkinUpdate);
		seed_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&seed_p90);

		id_ump45.setup(XOR("paintkit id"), XOR("id_ump45"), 3);
		id_ump45.SetCallback(callbacks::SkinUpdate);
		id_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&id_ump45);

		stattrak_ump45.setup(XOR("stattrak"), XOR("stattrak_ump45"));
		stattrak_ump45.SetCallback(callbacks::SkinUpdate);
		stattrak_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&stattrak_ump45);

		quality_ump45.setup(XOR("quality"), XOR("quality_ump45"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ump45.SetCallback(callbacks::SkinUpdate);
		quality_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&quality_ump45);

		seed_ump45.setup(XOR("seed"), XOR("seed_ump45"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ump45.SetCallback(callbacks::SkinUpdate);
		seed_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&seed_ump45);

		id_xm1014.setup(XOR("paintkit id"), XOR("id_xm1014"), 3);
		id_xm1014.SetCallback(callbacks::SkinUpdate);
		id_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&id_xm1014);

		stattrak_xm1014.setup(XOR("stattrak"), XOR("stattrak_xm1014"));
		stattrak_xm1014.SetCallback(callbacks::SkinUpdate);
		stattrak_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&stattrak_xm1014);

		quality_xm1014.setup(XOR("quality"), XOR("quality_xm1014"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_xm1014.SetCallback(callbacks::SkinUpdate);
		quality_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&quality_xm1014);

		seed_xm1014.setup(XOR("seed"), XOR("seed_xm1014"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_xm1014.SetCallback(callbacks::SkinUpdate);
		seed_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&seed_xm1014);

		id_bizon.setup(XOR("paintkit id"), XOR("id_bizon"), 3);
		id_bizon.SetCallback(callbacks::SkinUpdate);
		id_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&id_bizon);

		stattrak_bizon.setup(XOR("stattrak"), XOR("stattrak_bizon"));
		stattrak_bizon.SetCallback(callbacks::SkinUpdate);
		stattrak_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&stattrak_bizon);

		quality_bizon.setup(XOR("quality"), XOR("quality_bizon"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bizon.SetCallback(callbacks::SkinUpdate);
		quality_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&quality_bizon);

		seed_bizon.setup(XOR("seed"), XOR("seed_bizon"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bizon.SetCallback(callbacks::SkinUpdate);
		seed_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&seed_bizon);

		id_mag7.setup(XOR("paintkit id"), XOR("id_mag7"), 3);
		id_mag7.SetCallback(callbacks::SkinUpdate);
		id_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&id_mag7);

		stattrak_mag7.setup(XOR("stattrak"), XOR("stattrak_mag7"));
		stattrak_mag7.SetCallback(callbacks::SkinUpdate);
		stattrak_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&stattrak_mag7);

		quality_mag7.setup(XOR("quality"), XOR("quality_mag7"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mag7.SetCallback(callbacks::SkinUpdate);
		quality_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&quality_mag7);

		seed_mag7.setup(XOR("seed"), XOR("seed_mag7"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mag7.SetCallback(callbacks::SkinUpdate);
		seed_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&seed_mag7);

		mag7_color.setup(XOR("color"), XOR("mag7_color"), colors::white);
		mag7_color.SetCallback(callbacks::SkinUpdate);
		mag7_color.AddShowCallback(callbacks::MAG7);
		RegisterElement(&mag7_color);

		mag7_color1.setup(XOR("color 1"), XOR("mag7_color1"), colors::white);
		mag7_color1.SetCallback(callbacks::SkinUpdate);
		mag7_color1.AddShowCallback(callbacks::MAG7);
		RegisterElement(&mag7_color1);

		mag7_color2.setup(XOR("color 2"), XOR("mag7_color2"), colors::white);
		mag7_color2.SetCallback(callbacks::SkinUpdate);
		mag7_color2.AddShowCallback(callbacks::MAG7);
		RegisterElement(&mag7_color2);

		mag7_color3.setup(XOR("color 3"), XOR("mag7_color3"), colors::white);
		mag7_color3.SetCallback(callbacks::SkinUpdate);
		mag7_color3.AddShowCallback(callbacks::MAG7);
		RegisterElement(&mag7_color3);

		id_negev.setup(XOR("paintkit id"), XOR("id_negev"), 3);
		id_negev.SetCallback(callbacks::SkinUpdate);
		id_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&id_negev);

		stattrak_negev.setup(XOR("stattrak"), XOR("stattrak_negev"));
		stattrak_negev.SetCallback(callbacks::SkinUpdate);
		stattrak_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&stattrak_negev);

		quality_negev.setup(XOR("quality"), XOR("quality_negev"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_negev.SetCallback(callbacks::SkinUpdate);
		quality_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&quality_negev);

		seed_negev.setup(XOR("seed"), XOR("seed_negev"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_negev.SetCallback(callbacks::SkinUpdate);
		seed_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&seed_negev);

		id_sawedoff.setup(XOR("paintkit id"), XOR("id_sawedoff"), 3);
		id_sawedoff.SetCallback(callbacks::SkinUpdate);
		id_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&id_sawedoff);

		stattrak_sawedoff.setup(XOR("stattrak"), XOR("stattrak_sawedoff"));
		stattrak_sawedoff.SetCallback(callbacks::SkinUpdate);
		stattrak_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&stattrak_sawedoff);

		quality_sawedoff.setup(XOR("quality"), XOR("quality_sawedoff"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_sawedoff.SetCallback(callbacks::SkinUpdate);
		quality_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&quality_sawedoff);

		seed_sawedoff.setup(XOR("seed"), XOR("seed_sawedoff"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_sawedoff.SetCallback(callbacks::SkinUpdate);
		seed_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&seed_sawedoff);

		id_tec9.setup(XOR("paintkit id"), XOR("id_tec9"), 3);
		id_tec9.SetCallback(callbacks::SkinUpdate);
		id_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&id_tec9);

		stattrak_tec9.setup(XOR("stattrak"), XOR("stattrak_tec9"));
		stattrak_tec9.SetCallback(callbacks::SkinUpdate);
		stattrak_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&stattrak_tec9);

		quality_tec9.setup(XOR("quality"), XOR("quality_tec9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_tec9.SetCallback(callbacks::SkinUpdate);
		quality_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&quality_tec9);

		seed_tec9.setup(XOR("seed"), XOR("seed_tec9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_tec9.SetCallback(callbacks::SkinUpdate);
		seed_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&seed_tec9);

		id_p2000.setup(XOR("paintkit id"), XOR("id_p2000"), 3);
		id_p2000.SetCallback(callbacks::SkinUpdate);
		id_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&id_p2000);

		stattrak_p2000.setup(XOR("stattrak"), XOR("stattrak_p2000"));
		stattrak_p2000.SetCallback(callbacks::SkinUpdate);
		stattrak_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&stattrak_p2000);

		quality_p2000.setup(XOR("quality"), XOR("quality_p2000"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p2000.SetCallback(callbacks::SkinUpdate);
		quality_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&quality_p2000);

		seed_p2000.setup(XOR("seed"), XOR("seed_p2000"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p2000.SetCallback(callbacks::SkinUpdate);
		seed_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&seed_p2000);

		p2000_color.setup(XOR("color"), XOR("p2000_color"), colors::white);
		p2000_color.SetCallback(callbacks::SkinUpdate);
		p2000_color.AddShowCallback(callbacks::P2000);
		RegisterElement(&p2000_color);

		p2000_color1.setup(XOR("color 1"), XOR("p2000_color1"), colors::white);
		p2000_color1.SetCallback(callbacks::SkinUpdate);
		p2000_color1.AddShowCallback(callbacks::P2000);
		RegisterElement(&p2000_color1);

		p2000_color2.setup(XOR("color 2"), XOR("p2000_color2"), colors::white);
		p2000_color2.SetCallback(callbacks::SkinUpdate);
		p2000_color2.AddShowCallback(callbacks::P2000);
		RegisterElement(&p2000_color2);

		p2000_color3.setup(XOR("color 3"), XOR("p2000_color3"), colors::white);
		p2000_color3.SetCallback(callbacks::SkinUpdate);
		p2000_color3.AddShowCallback(callbacks::P2000);
		RegisterElement(&p2000_color3);

		id_mp7.setup(XOR("paintkit id"), XOR("id_mp7"), 3);
		id_mp7.SetCallback(callbacks::SkinUpdate);
		id_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&id_mp7);

		stattrak_mp7.setup(XOR("stattrak"), XOR("stattrak_mp7"));
		stattrak_mp7.SetCallback(callbacks::SkinUpdate);
		stattrak_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&stattrak_mp7);

		quality_mp7.setup(XOR("quality"), XOR("quality_mp7"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mp7.SetCallback(callbacks::SkinUpdate);
		quality_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&quality_mp7);

		seed_mp7.setup(XOR("seed"), XOR("seed_mp7"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mp7.SetCallback(callbacks::SkinUpdate);
		seed_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&seed_mp7);

		id_mp9.setup(XOR("paintkit id"), XOR("id_mp9"), 3);
		id_mp9.SetCallback(callbacks::SkinUpdate);
		id_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&id_mp9);

		stattrak_mp9.setup(XOR("stattrak"), XOR("stattrak_mp9"));
		stattrak_mp9.SetCallback(callbacks::SkinUpdate);
		stattrak_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&stattrak_mp9);

		quality_mp9.setup(XOR("quality"), XOR("quality_mp9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mp9.SetCallback(callbacks::SkinUpdate);
		quality_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&quality_mp9);

		seed_mp9.setup(XOR("seed"), XOR("seed_mp9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mp9.SetCallback(callbacks::SkinUpdate);
		seed_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&seed_mp9);

		id_nova.setup(XOR("paintkit id"), XOR("id_nova"), 3);
		id_nova.SetCallback(callbacks::SkinUpdate);
		id_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&id_nova);

		stattrak_nova.setup(XOR("stattrak"), XOR("stattrak_nova"));
		stattrak_nova.SetCallback(callbacks::SkinUpdate);
		stattrak_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&stattrak_nova);

		quality_nova.setup(XOR("quality"), XOR("quality_nova"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_nova.SetCallback(callbacks::SkinUpdate);
		quality_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&quality_nova);

		seed_nova.setup(XOR("seed"), XOR("seed_nova"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_nova.SetCallback(callbacks::SkinUpdate);
		seed_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&seed_nova);

		id_p250.setup(XOR("paintkit id"), XOR("id_p250"), 3);
		id_p250.SetCallback(callbacks::SkinUpdate);
		id_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&id_p250);

		stattrak_p250.setup(XOR("stattrak"), XOR("stattrak_p250"));
		stattrak_p250.SetCallback(callbacks::SkinUpdate);
		stattrak_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&stattrak_p250);

		quality_p250.setup(XOR("quality"), XOR("quality_p250"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p250.SetCallback(callbacks::SkinUpdate);
		quality_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&quality_p250);

		seed_p250.setup(XOR("seed"), XOR("seed_p250"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p250.SetCallback(callbacks::SkinUpdate);
		seed_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&seed_p250);

		id_scar20.setup(XOR("paintkit id"), XOR("id_scar20"), 3);
		id_scar20.SetCallback(callbacks::SkinUpdate);
		id_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&id_scar20);

		stattrak_scar20.setup(XOR("stattrak"), XOR("stattrak_scar20"));
		stattrak_scar20.SetCallback(callbacks::SkinUpdate);
		stattrak_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&stattrak_scar20);

		quality_scar20.setup(XOR("quality"), XOR("quality_scar20"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_scar20.SetCallback(callbacks::SkinUpdate);
		quality_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&quality_scar20);

		seed_scar20.setup(XOR("seed"), XOR("seed_scar20"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_scar20.SetCallback(callbacks::SkinUpdate);
		seed_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&seed_scar20);

		scar20_color.setup(XOR("color"), XOR("scar20_color"), colors::white);
		scar20_color.SetCallback(callbacks::SkinUpdate);
		scar20_color.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&scar20_color);

		scar20_color1.setup(XOR("color 1"), XOR("scar20_color1"), colors::white);
		scar20_color1.SetCallback(callbacks::SkinUpdate);
		scar20_color1.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&scar20_color1);

		scar20_color2.setup(XOR("color 2"), XOR("scar20_color2"), colors::white);
		scar20_color2.SetCallback(callbacks::SkinUpdate);
		scar20_color2.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&scar20_color2);

		scar20_color3.setup(XOR("color 3"), XOR("scar20_color3"), colors::white);
		scar20_color3.SetCallback(callbacks::SkinUpdate);
		scar20_color3.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&scar20_color3);

		id_sg553.setup(XOR("paintkit id"), XOR("id_sg553"), 3);
		id_sg553.SetCallback(callbacks::SkinUpdate);
		id_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&id_sg553);

		stattrak_sg553.setup(XOR("stattrak"), XOR("stattrak_sg553"));
		stattrak_sg553.SetCallback(callbacks::SkinUpdate);
		stattrak_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&stattrak_sg553);

		quality_sg553.setup(XOR("quality"), XOR("quality_sg553"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_sg553.SetCallback(callbacks::SkinUpdate);
		quality_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&quality_sg553);

		seed_sg553.setup(XOR("seed"), XOR("seed_sg553"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_sg553.SetCallback(callbacks::SkinUpdate);
		seed_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&seed_sg553);

		id_ssg08.setup(XOR("paintkit id"), XOR("id_ssg08"), 3);
		id_ssg08.SetCallback(callbacks::SkinUpdate);
		id_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&id_ssg08);

		stattrak_ssg08.setup(XOR("stattrak"), XOR("stattrak_ssg08"));
		stattrak_ssg08.SetCallback(callbacks::SkinUpdate);
		stattrak_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&stattrak_ssg08);

		quality_ssg08.setup(XOR("quality"), XOR("quality_ssg08"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ssg08.SetCallback(callbacks::SkinUpdate);
		quality_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&quality_ssg08);

		seed_ssg08.setup(XOR("seed"), XOR("seed_ssg08"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ssg08.SetCallback(callbacks::SkinUpdate);
		seed_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&seed_ssg08);

		ssg08_color.setup(XOR("color"), XOR("ssg08_color"), colors::white);
		ssg08_color.SetCallback(callbacks::SkinUpdate);
		ssg08_color.AddShowCallback(callbacks::SSG08);
		RegisterElement(&ssg08_color);

		ssg08_color1.setup(XOR("color 1"), XOR("ssg08_color1"), colors::white);
		ssg08_color1.SetCallback(callbacks::SkinUpdate);
		ssg08_color1.AddShowCallback(callbacks::SSG08);
		RegisterElement(&ssg08_color1);

		ssg08_color2.setup(XOR("color 2"), XOR("ssg08_color2"), colors::white);
		ssg08_color2.SetCallback(callbacks::SkinUpdate);
		ssg08_color2.AddShowCallback(callbacks::SSG08);
		RegisterElement(&ssg08_color2);

		ssg08_color3.setup(XOR("color 3"), XOR("ssg08_color3"), colors::white);
		ssg08_color3.SetCallback(callbacks::SkinUpdate);
		ssg08_color3.AddShowCallback(callbacks::SSG08);
		RegisterElement(&ssg08_color3);

		id_m4a1s.setup(XOR("paintkit id"), XOR("id_m4a1s"), 3);
		id_m4a1s.SetCallback(callbacks::SkinUpdate);
		id_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&id_m4a1s);

		stattrak_m4a1s.setup(XOR("stattrak"), XOR("stattrak_m4a1s"));
		stattrak_m4a1s.SetCallback(callbacks::SkinUpdate);
		stattrak_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&stattrak_m4a1s);

		quality_m4a1s.setup(XOR("quality"), XOR("quality_m4a1s"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m4a1s.SetCallback(callbacks::SkinUpdate);
		quality_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&quality_m4a1s);

		seed_m4a1s.setup(XOR("seed"), XOR("seed_m4a1s"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m4a1s.SetCallback(callbacks::SkinUpdate);
		seed_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&seed_m4a1s);

		id_usps.setup(XOR("paintkit id"), XOR("id_usps"), 3);
		id_usps.SetCallback(callbacks::SkinUpdate);
		id_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&id_usps);

		stattrak_usps.setup(XOR("stattrak"), XOR("stattrak_usps"));
		stattrak_usps.SetCallback(callbacks::SkinUpdate);
		stattrak_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&stattrak_usps);

		quality_usps.setup(XOR("quality"), XOR("quality_usps"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_usps.SetCallback(callbacks::SkinUpdate);
		quality_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&quality_usps);

		seed_usps.setup(XOR("seed"), XOR("seed_usps"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_usps.SetCallback(callbacks::SkinUpdate);
		seed_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&seed_usps);

		id_cz75a.setup(XOR("paintkit id"), XOR("id_cz75a"), 3);
		id_cz75a.SetCallback(callbacks::SkinUpdate);
		id_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&id_cz75a);

		stattrak_cz75a.setup(XOR("stattrak"), XOR("stattrak_cz75a"));
		stattrak_cz75a.SetCallback(callbacks::SkinUpdate);
		stattrak_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&stattrak_cz75a);

		quality_cz75a.setup(XOR("quality"), XOR("quality_cz75a"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_cz75a.SetCallback(callbacks::SkinUpdate);
		quality_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&quality_cz75a);

		seed_cz75a.setup(XOR("seed"), XOR("seed_cz75a"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_cz75a.SetCallback(callbacks::SkinUpdate);
		seed_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&seed_cz75a);

		id_revolver.setup(XOR("paintkit id"), XOR("id_revolver"), 3);
		id_revolver.SetCallback(callbacks::SkinUpdate);
		id_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&id_revolver);

		stattrak_revolver.setup(XOR("stattrak"), XOR("stattrak_revolver"));
		stattrak_revolver.SetCallback(callbacks::SkinUpdate);
		stattrak_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&stattrak_revolver);

		quality_revolver.setup(XOR("quality"), XOR("quality_revolver"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_revolver.SetCallback(callbacks::SkinUpdate);
		quality_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&quality_revolver);

		seed_revolver.setup(XOR("seed"), XOR("seed_revolver"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_revolver.SetCallback(callbacks::SkinUpdate);
		seed_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&seed_revolver);

		id_bayonet.setup(XOR("paintkit id"), XOR("id_bayonet"), 3);
		id_bayonet.SetCallback(callbacks::SkinUpdate);
		id_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&id_bayonet);

		stattrak_bayonet.setup(XOR("stattrak"), XOR("stattrak_bayonet"));
		stattrak_bayonet.SetCallback(callbacks::SkinUpdate);
		stattrak_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&stattrak_bayonet);

		quality_bayonet.setup(XOR("quality"), XOR("quality_bayonet"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bayonet.SetCallback(callbacks::SkinUpdate);
		quality_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&quality_bayonet);

		seed_bayonet.setup(XOR("seed"), XOR("seed_bayonet"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bayonet.SetCallback(callbacks::SkinUpdate);
		seed_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&seed_bayonet);

		id_flip.setup(XOR("paintkit id"), XOR("id_flip"), 3);
		id_flip.SetCallback(callbacks::SkinUpdate);
		id_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&id_flip);

		stattrak_flip.setup(XOR("stattrak"), XOR("stattrak_flip"));
		stattrak_flip.SetCallback(callbacks::SkinUpdate);
		stattrak_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&stattrak_flip);

		quality_flip.setup(XOR("quality"), XOR("quality_flip"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_flip.SetCallback(callbacks::SkinUpdate);
		quality_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&quality_flip);

		seed_flip.setup(XOR("seed"), XOR("seed_flip"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_flip.SetCallback(callbacks::SkinUpdate);
		seed_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&seed_flip);

		id_gut.setup(XOR("paintkit id"), XOR("id_gut"), 3);
		id_gut.SetCallback(callbacks::SkinUpdate);
		id_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&id_gut);

		stattrak_gut.setup(XOR("stattrak"), XOR("stattrak_gut"));
		stattrak_gut.SetCallback(callbacks::SkinUpdate);
		stattrak_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&stattrak_gut);

		quality_gut.setup(XOR("quality"), XOR("quality_gut"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_gut.SetCallback(callbacks::SkinUpdate);
		quality_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&quality_gut);

		seed_gut.setup(XOR("seed"), XOR("seed_gut"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_gut.SetCallback(callbacks::SkinUpdate);
		seed_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&seed_gut);

		id_karambit.setup(XOR("paintkit id"), XOR("id_karambit"), 3);
		id_karambit.SetCallback(callbacks::SkinUpdate);
		id_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&id_karambit);

		stattrak_karambit.setup(XOR("stattrak"), XOR("stattrak_karambit"));
		stattrak_karambit.SetCallback(callbacks::SkinUpdate);
		stattrak_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&stattrak_karambit);

		quality_karambit.setup(XOR("quality"), XOR("quality_karambit"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_karambit.SetCallback(callbacks::SkinUpdate);
		quality_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&quality_karambit);

		seed_karambit.setup(XOR("seed"), XOR("seed_karambit"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_karambit.SetCallback(callbacks::SkinUpdate);
		seed_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&seed_karambit);

		id_m9.setup(XOR("paintkit id"), XOR("id_m9"), 3);
		id_m9.SetCallback(callbacks::SkinUpdate);
		id_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&id_m9);

		stattrak_m9.setup(XOR("stattrak"), XOR("stattrak_m9"));
		stattrak_m9.SetCallback(callbacks::SkinUpdate);
		stattrak_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&stattrak_m9);

		quality_m9.setup(XOR("quality"), XOR("quality_m9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m9.SetCallback(callbacks::SkinUpdate);
		quality_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&quality_m9);

		seed_m9.setup(XOR("seed"), XOR("seed_m9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m9.SetCallback(callbacks::SkinUpdate);
		seed_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&seed_m9);

		id_huntsman.setup(XOR("paintkit id"), XOR("id_huntsman"), 3);
		id_huntsman.SetCallback(callbacks::SkinUpdate);
		id_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&id_huntsman);

		stattrak_huntsman.setup(XOR("stattrak"), XOR("stattrak_huntsman"));
		stattrak_huntsman.SetCallback(callbacks::SkinUpdate);
		stattrak_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&stattrak_huntsman);

		quality_huntsman.setup(XOR("quality"), XOR("quality_huntsman"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_huntsman.SetCallback(callbacks::SkinUpdate);
		quality_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&quality_huntsman);

		seed_huntsman.setup(XOR("seed"), XOR("seed_huntsman"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_huntsman.SetCallback(callbacks::SkinUpdate);
		seed_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&seed_huntsman);

		id_falchion.setup(XOR("paintkit id"), XOR("id_falchion"), 3);
		id_falchion.SetCallback(callbacks::SkinUpdate);
		id_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&id_falchion);

		stattrak_falchion.setup(XOR("stattrak"), XOR("stattrak_falchion"));
		stattrak_falchion.SetCallback(callbacks::SkinUpdate);
		stattrak_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&stattrak_falchion);

		quality_falchion.setup(XOR("quality"), XOR("quality_falchion"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_falchion.SetCallback(callbacks::SkinUpdate);
		quality_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&quality_falchion);

		seed_falchion.setup(XOR("seed"), XOR("seed_falchion"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_falchion.SetCallback(callbacks::SkinUpdate);
		seed_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&seed_falchion);

		id_bowie.setup(XOR("paintkit id"), XOR("id_bowie"), 3);
		id_bowie.SetCallback(callbacks::SkinUpdate);
		id_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&id_bowie);

		stattrak_bowie.setup(XOR("stattrak"), XOR("stattrak_bowie"));
		stattrak_bowie.SetCallback(callbacks::SkinUpdate);
		stattrak_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&stattrak_bowie);

		quality_bowie.setup(XOR("quality"), XOR("quality_bowie"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bowie.SetCallback(callbacks::SkinUpdate);
		quality_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&quality_bowie);

		seed_bowie.setup(XOR("seed"), XOR("seed_bowie"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bowie.SetCallback(callbacks::SkinUpdate);
		seed_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&seed_bowie);

		id_butterfly.setup(XOR("paintkit id"), XOR("id_butterfly"), 3);
		id_butterfly.SetCallback(callbacks::SkinUpdate);
		id_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&id_butterfly);

		stattrak_butterfly.setup(XOR("stattrak"), XOR("stattrak_butterfly"));
		stattrak_butterfly.SetCallback(callbacks::SkinUpdate);
		stattrak_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&stattrak_butterfly);

		quality_butterfly.setup(XOR("quality"), XOR("quality_butterfly"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_butterfly.SetCallback(callbacks::SkinUpdate);
		quality_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&quality_butterfly);

		seed_butterfly.setup(XOR("seed"), XOR("seed_butterfly"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_butterfly.SetCallback(callbacks::SkinUpdate);
		seed_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&seed_butterfly);

		id_daggers.setup(XOR("paintkit id"), XOR("id_daggers"), 3);
		id_daggers.SetCallback(callbacks::SkinUpdate);
		id_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&id_daggers);

		stattrak_daggers.setup(XOR("stattrak"), XOR("stattrak_daggers"));
		stattrak_daggers.SetCallback(callbacks::SkinUpdate);
		stattrak_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&stattrak_daggers);

		quality_daggers.setup(XOR("quality"), XOR("quality_daggers"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_daggers.SetCallback(callbacks::SkinUpdate);
		quality_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&quality_daggers);

		seed_daggers.setup(XOR("seed"), XOR("seed_daggers"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_daggers.SetCallback(callbacks::SkinUpdate);
		seed_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&seed_daggers);

		// col 2.
		knife.setup(XOR("knife model"), XOR("skins_knife_model"), { XOR("off"), XOR("bayonet"), XOR("bowie"), XOR("butterfly"), XOR("falchion"), XOR("flip"), XOR("gut"), XOR("huntsman"), XOR("karambit"), XOR("m9 bayonet"), XOR("daggers") });
		knife.SetCallback(callbacks::SkinUpdate);
		RegisterElement(&knife, 1);

		glove.setup(XOR("glove model"), XOR("skins_glove_model"), { XOR("off"), XOR("bloodhound"), XOR("sport"), XOR("driver"), XOR("handwraps"), XOR("moto"), XOR("specialist") });
		glove.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&glove, 1);

		glove_id.setup(XOR("glove paintkit id"), XOR("skins_glove_id"), 2);
		glove_id.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&glove_id, 1);

	}
};

class MiscTab : public Tab {
public:
	// col1.
	MultiDropdown buy1;
	MultiDropdown buy2;
	MultiDropdown buy3;
	MultiDropdown notifications;

	Keybind       last_tick_defuse;
	Checkbox      force_fake_latency;
	Slider		  forced_fake_latency_amt;
	Keybind       sec_ping_spike;
	Slider		  sec_ping_spike_amt;
	Keybind       fake_latency;
	Slider		  fake_latency_amt;

	Checkbox      clantag;
	Edit          clantag_text;

	// col2.
	Checkbox skyboxchange;
	Dropdown skybox;
	Checkbox nospread;
	Checkbox autoaccept;
	Checkbox interpolate_local;
	Checkbox dont_render_body_update;
	Checkbox interpolate_enemy;
	Checkbox unlock;
	MultiDropdown hitmarker;
	Checkbox ragdoll_force;
	Checkbox killfeed;
	Checkbox ranks;
	Checkbox watermark;
	Dropdown cheat_mode;

public:
	void init() {
		SetTitle(XOR("misc"));

		buy1.setup(XOR("auto buy items"), XOR("auto_buy1"),
			{
				XOR("galilar"),
				XOR("famas"),
				XOR("ak47"),
				XOR("m4a1"),
				XOR("m4a1_silencer"),
				XOR("ssg08"),
				XOR("aug"),
				XOR("sg556"),
				XOR("awp"),
				XOR("scar20"),
				XOR("g3sg1"),
				XOR("nova"),
				XOR("xm1014"),
				XOR("mag7"),
				XOR("m249"),
				XOR("negev"),
				XOR("mac10"),
				XOR("mp9"),
				XOR("mp7"),
				XOR("ump45"),
				XOR("p90"),
				XOR("bizon"),
			});
		RegisterElement(&buy1);

		buy2.setup("", XOR("auto_buy2"),
			{
				XOR("glock"),
				XOR("hkp2000"),
				XOR("usp_silencer"),
				XOR("elite"),
				XOR("p250"),
				XOR("tec9"),
				XOR("fn57"),
				XOR("deagle"),
			}, false);
		RegisterElement(&buy2);

		buy3.setup("", XOR("auto_buy3"),
			{
				XOR("vest"),
				XOR("vesthelm"),
				XOR("taser"),
				XOR("defuser"),
				XOR("heavyarmor"),
				XOR("molotov"),
				XOR("incgrenade"),
				XOR("decoy"),
				XOR("flashbang"),
				XOR("hegrenade"),
				XOR("smokegrenade"),
			}, false);
		RegisterElement(&buy3);

		notifications.setup(XOR("notifications"), XOR("notifications"), { XOR("matchmaking"), XOR("damage"), XOR("shot"), XOR("purchases"), XOR("bomb"), XOR("defuse") });
		RegisterElement(&notifications);

		last_tick_defuse.setup(XOR("last tick defuse"), XOR("last_tick_defuse"));
		RegisterElement(&last_tick_defuse);

		fake_latency.setup(XOR("fake latency"), XOR("fake_latency"));
		fake_latency.SetToggleCallback(callbacks::ToggleFakeLatency);
		RegisterElement(&fake_latency);

		fake_latency_amt.setup("", XOR("fake_latency_amt"), 10.f, 1000.f, false, 0, 200.f, 10.f, XOR(L"ms"));
		RegisterElement(&fake_latency_amt);

		force_fake_latency.setup(XOR("force fake latency"), XOR("force_fake_latency"));
		RegisterElement(&force_fake_latency);

		forced_fake_latency_amt.setup("", XOR("forced_fake_latency_amt"), 10.f, 1000.f, false, 0, 200.f, 10.f, XOR(L"ms"));
		forced_fake_latency_amt.AddShowCallback(callbacks::ForceFakeLatency);
		RegisterElement(&forced_fake_latency_amt);

		// col2.

		skyboxchange.setup(XOR("skybox changer"), XOR("skyboxchange"));
		RegisterElement(&skyboxchange, 1);

		skybox.setup(XOR("skyboxes"), XOR("skybox"), { XOR("Tibet"),XOR("Embassy"),XOR("Italy"),XOR("Daylight"),XOR("Cloudy"),XOR("Night 1"),XOR("Night 2"),XOR("Night Flat"),XOR("Day HD"),XOR("Day"),XOR("Rural"),XOR("Vertigo HD"),XOR("Vertigo Blue HD"),XOR("Vertigo"),XOR("Vietnam"),XOR("Dusty Sky"),XOR("Jungle"),XOR("Nuke"),XOR("Office") });
		skybox.AddShowCallback(callbacks::IsSkyBoxChange);

		RegisterElement(&skybox, 1);

		nospread.setup(XOR("no spread"), XOR("nospread"));
		RegisterElement(&nospread, 1);


		clantag.setup(XOR("clan-tag"), XOR("clantag"));
		RegisterElement(&clantag, 1);

		clantag_text.setup(XOR("tag"), XOR("clantag_text"), 14);
		RegisterElement(&clantag_text, 1);

		watermark.setup(XOR("watermark"), XOR("watermark"));
		RegisterElement(&watermark, 1);

		autoaccept.setup(XOR("auto-accept matchmaking"), XOR("autoaccept"));
		RegisterElement(&autoaccept, 1);

		interpolate_local.setup(XOR("interpolate local animations"), XOR("interpolate_local"));
		RegisterElement(&interpolate_local, 1);

		dont_render_body_update.setup(XOR("remove body update"), XOR("dont_render_body_update"));
		RegisterElement(&dont_render_body_update, 1);

		/*	interpolate_enemy.setup(XOR("interpolate enemy origin"), XOR("interpolate_enemy"));
			RegisterElement(&interpolate_enemy, 1);*/

		unlock.setup(XOR("unlock inventory in-game"), XOR("unlock_inventory"));
		RegisterElement(&unlock, 1);

		hitmarker.setup(XOR("hitmarker"), XOR("hitmarker"), { XOR("normal"), XOR("world") });
		RegisterElement(&hitmarker, 1);

		ragdoll_force.setup(XOR("ragdoll force"), XOR("ragdoll_force"));
		RegisterElement(&ragdoll_force, 1);

		ranks.setup(XOR("reveal matchmaking ranks"), XOR("ranks"));
		RegisterElement(&ranks, 1);

		killfeed.setup(XOR("preserve killfeed"), XOR("killfeed"));
		killfeed.SetCallback(callbacks::ToggleKillfeed);
		RegisterElement(&killfeed, 1);

		cheat_mode.setup("mode", XOR("cheat_mode"),
			{
				XOR("source"),
				XOR("family"),
			}, true);
		RegisterElement(&cheat_mode, 1);

	}
};

class ConfigTab : public Tab {
public:
	Colorpicker menu_color;

	Dropdown mode;
	Dropdown config;
	Keybind  key1;
	Keybind  key2;
	Keybind  key3;
	Keybind  key4;
	Keybind  key5;
	Keybind  key6;
	Button   save;
	Button   load;

public:

	void init() {
		SetTitle(XOR("config"));

		menu_color.setup(XOR("menu color"), XOR("menu_color"), colors::burgundy, &g_gui.m_color);
		RegisterElement(&menu_color);

		mode.setup(XOR("safety mode"), XOR("mode"), { XOR("matchmaking"), XOR("no-spread") });
		RegisterElement(&mode, 1);

		config.setup(XOR("configuration"), XOR("cfg"), { XOR("1"), XOR("2"), XOR("3"), XOR("4"), XOR("5"), XOR("6") });
		config.RemoveFlags(ElementFlags::SAVE);
		RegisterElement(&config, 1);

		key1.setup(XOR("configuration key 1"), XOR("cfg_key1"));
		key1.RemoveFlags(ElementFlags::SAVE);
		key1.SetCallback(callbacks::SaveHotkeys);
		key1.AddShowCallback(callbacks::IsConfig1);
		key1.SetToggleCallback(callbacks::ConfigLoad1);
		RegisterElement(&key1, 1);

		key2.setup(XOR("configuration key 2"), XOR("cfg_key2"));
		key2.RemoveFlags(ElementFlags::SAVE);
		key2.SetCallback(callbacks::SaveHotkeys);
		key2.AddShowCallback(callbacks::IsConfig2);
		key2.SetToggleCallback(callbacks::ConfigLoad2);
		RegisterElement(&key2, 1);

		key3.setup(XOR("configuration key 3"), XOR("cfg_key3"));
		key3.RemoveFlags(ElementFlags::SAVE);
		key3.SetCallback(callbacks::SaveHotkeys);
		key3.AddShowCallback(callbacks::IsConfig3);
		key3.SetToggleCallback(callbacks::ConfigLoad3);
		RegisterElement(&key3, 1);

		key4.setup(XOR("configuration key 4"), XOR("cfg_key4"));
		key4.RemoveFlags(ElementFlags::SAVE);
		key4.SetCallback(callbacks::SaveHotkeys);
		key4.AddShowCallback(callbacks::IsConfig4);
		key4.SetToggleCallback(callbacks::ConfigLoad4);
		RegisterElement(&key4, 1);

		key5.setup(XOR("configuration key 5"), XOR("cfg_key5"));
		key5.RemoveFlags(ElementFlags::SAVE);
		key5.SetCallback(callbacks::SaveHotkeys);
		key5.AddShowCallback(callbacks::IsConfig5);
		key5.SetToggleCallback(callbacks::ConfigLoad5);
		RegisterElement(&key5, 1);

		key6.setup(XOR("configuration key 6"), XOR("cfg_key6"));
		key6.RemoveFlags(ElementFlags::SAVE);
		key6.SetCallback(callbacks::SaveHotkeys);
		key6.AddShowCallback(callbacks::IsConfig6);
		key6.SetToggleCallback(callbacks::ConfigLoad6);
		RegisterElement(&key6, 1);

		save.setup(XOR("save"));
		save.SetCallback(callbacks::ConfigSave);
		RegisterElement(&save, 1);

		load.setup(XOR("load"));
		load.SetCallback(callbacks::ConfigLoad);
		RegisterElement(&load, 1);
	}
};

class WhitelistTab : public Tab {
public:
	Edit   id;
	Button add;
	Button remove;
	Button clear;

public:

	void init() {
		SetTitle(XOR("whitelist"));

		id.setup(XOR("player id"), XOR("id"), 2);
		RegisterElement(&id);

		add.setup(XOR("add"));
		add.SetCallback(callbacks::WhitelistAdd);
		RegisterElement(&add);

		remove.setup(XOR("remove"));
		remove.SetCallback(callbacks::WhitelistRemove);
		RegisterElement(&remove);

		clear.setup(XOR("clear"));
		clear.SetCallback(callbacks::WhitelistClear);
		RegisterElement(&clear);
	}
};

class MainForm : public Form {
public:
	// aimbot.
	AimbotTab    aimbot;
	AntiAimTab   antiaim;

	// visuals.
	PlayersTab	 players;
	VisualsTab	 visuals;

	// misc.
	MovementTab  movement;
	SkinsTab     skins;
	MiscTab	     misc;
	ConfigTab	 config;
	WhitelistTab whitelist;

public:
	void init() {
		SetPosition(50, 50);
		SetSize(570, 490);

		// aim.
		RegisterTab(&aimbot);
		aimbot.init();

		RegisterTab(&antiaim);
		antiaim.init();

		// visuals.
		RegisterTab(&players);
		players.init();

		RegisterTab(&visuals);
		visuals.init();

		// misc.
		RegisterTab(&movement);
		movement.init();

		RegisterTab(&skins);
		skins.init();

		RegisterTab(&misc);
		misc.init();

		RegisterTab(&config);
		config.init();

		RegisterTab(&whitelist);
		whitelist.init();
	}
};

class Menu {
public:
	MainForm main;

public:
	void init() {
		main.init();
		g_gui.RegisterForm(&main, VK_INSERT);
	}
};

extern Menu g_menu;