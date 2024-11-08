/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const int startwithgaps[]    = { 1 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]   = { 10 };   /* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 10;       /* horizontal padding for statusbar */
static const int vertpadbar         = 0;        /* vertical padding for statusbar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font Mono:size=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font Mono:size=12:antialias=true:autohint=true";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static const char cat_base[]        = "#1e1e2e"; // catppuccin mocha base
static const char cat_surface1[]    = "#45475a"; // catppuccin mocha surface1
static const char cat_mantle[]      = "#181825"; // catppuccin mocha mantle
static const char cat_blue[]        = "#89b4fa"; // catppuccin mocha blue

static const char *colors[][3]      = {
	/*               fg            bg          border   */
	[SchemeNorm] = { cat_surface1, cat_mantle, cat_mantle   },
	[SchemeSel]  = { cat_blue,     cat_base,   cat_surface1 },
};

/* staticstatus */
static const int statmonval = 0;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                        instance                   title            tags mask     iscentered   isfloating   monitor */
	{ "Gcr-prompter",               "gcr-prompter",            NULL,            0,            1,           1,           0 },
	{ "Brave-browser",              NULL,                      NULL,            0,            0,           0,          -1 },
	{ "1Password",                  "1password",               NULL,            0,            1,           1,           0 },

	{ "install4j-jclient-Launcher", "sun-awt-X11-XWindowPeer", "###focusableSwingPopup###", 0,0,           1,           -1 },
	{ "install4j-jclient-Launcher", "sun-awt-X11-XWindowPeer", NULL,            0,            0,           1,           -1 },

	{ "install4j-jclient-Launcher", "sun-awt-X11-XFramePeer",  "Tool tips",     1 << 8,       0,           1,           -1 },
	{ "install4j-jclient-Launcher", "sun-awt-X11-XFramePeer",  NULL,            1 << 8,       1,           1,           -1 },

	{ "install4j-jclient-Launcher", "sun-awt-X11-XDialogPeer", "Order Preview", 1 << 8,       1,           1,           -1 },
	{ "install4j-jclient-Launcher", "sun-awt-X11-XDialogPeer", NULL,            1 << 8,       1,           1,           -1 },

	{ "install4j-jclient-Launcher", NULL,                      NULL,            1 << 8,       1,           1,           2 },

	{ "tasty.javafx.launcher.LauncherFxApp", NULL,             NULL,            2 << 8,       0,           0,           2 },
	{ "tasty.javafx.launcher.LauncherFxApp", NULL,             "tastytrade",    2 << 8,       0,           0,           2 },

	{ "zoom",                       NULL,                      NULL,            0,            0,           1,           0 },

	{ "steam_app_0",                NULL,                      NULL,            0,            0,           1,          -1 },
	{ "steam",                      "steamwebhelper",    "Steam Games List",    0,            0,           0,          -1 },
	{ "steam",                      "steamwebhelper",          NULL,            0,            1,           1,          -1 },
	{ "Slack",                      NULL,                      NULL,            1 << 8,       0,           0,           0 },
	{ "Pavucontrol",                NULL,                      NULL,            0,            1,           1,           0 },
	{ "Blueman-manager",            NULL,                      NULL,            0,            1,           1,           0 },
	{ "Update-manager",             NULL,                      NULL,            0,            1,           1,           0 },
	{ "SpeedCrunch",                NULL,                      NULL,            0,            1,           1,          -1 },
	{ "flameshot",                  NULL,                      NULL,            0,            0,           0,           0 },
	{ "stalonetray",                NULL,                      NULL,            1 << 8,       0,           1,           0 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions Mod4=Super_L & Mod3=Super_R */
#define MODKEY Mod4Mask
#define ALTMOD Mod3Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ ALTMOD,                       KEY,      focusnthmon,    {.i  = TAG } }, \
	{ ALTMOD|ShiftMask,             KEY,      tagnthmon,      {.i  = TAG } },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", cat_mantle, "-nf", cat_surface1, "-sb", cat_base, "-sf", cat_surface1, "-nhb", cat_mantle, "-nhf", cat_blue, "-shb", cat_base, "-shf", cat_blue, NULL };
static const char *roficmd[]  = { "rofi", "-m", dmenumon, "-i", "-modi", "combi,window", "-show", "combi", "-combi-modi", "run,drun", "-show-icons", "-matching", "fuzzy", NULL };
static const char *termcmd[]  = { "x-terminal-emulator", NULL };
static const char *calccmd[]  = { "speedcrunch", NULL };
static const char *xprop[] = { "tmux", "set-option", "remain-on-exit", "on", ";", "split-window", "xprop", NULL };

static const char *muteMus[]  = { "wpctl", "set-mute",   "@DEFAULT_AUDIO_SINK@",   "toggle" };
static const char *muteMic[]  = { "wpctl", "set-mute",   "@DEFAULT_AUDIO_SOURCE@", "toggle" };
static const char *volumeUp[] = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@",   "3%+" };
static const char *volumeDn[] = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@",   "3%-" };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
 	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = calccmd } },
 	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = xprop } },

	{ 0,              XF86XK_AudioMute,        spawn,          {.v = muteMus } },
	{ 0,              XF86XK_AudioMicMute,     spawn,          {.v = muteMic } },
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = volumeUp } },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = volumeDn } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ ALTMOD,                       XK_comma,  focusmon,       {.i = -1 } },
	{ ALTMOD,                       XK_period, focusmon,       {.i = +1 } },
	{ ALTMOD|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ ALTMOD|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_Left,   viewtoleft,     {0} },
	{ MODKEY,                       XK_Right,  viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_Left,   tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_Right,  tagtoright,     {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	{ ALTMOD,                       XK_minus,  setborderpx,    {.i = -1 } },
	{ ALTMOD,                       XK_equal,  setborderpx,    {.i = +1 } },
	{ ALTMOD|ShiftMask,             XK_equal,  setborderpx,    {.i = 0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkMonNum,            0,              Button1,        focusmon,       {.i = +1} },
	{ ClkMonNum,            0,              Button3,        focusmon,       {.i = -1} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

