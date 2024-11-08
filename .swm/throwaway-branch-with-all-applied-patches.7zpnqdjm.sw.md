---
title: throwaway branch with all applied patches
---
# Introduction

This document will walk you through the implementation of the "throwaway branch with all applied patches" feature.

The feature introduces several enhancements and fixes to the window manager configuration and behavior.

We will cover:

1. Appearance and color scheme settings.
2. Key definitions and commands.
3. Client and monitor management.
4. Layout and gap management.
5. Event handling and focus management.

# Appearance and color scheme settings

The appearance settings define the visual aspects of the window manager, such as borders, gaps, and colors.

<SwmSnippet path="config.def.h" line="2">

---

We set the default appearance parameters, including border pixel size, gaps, and bar visibility:

```
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
```

---

</SwmSnippet>

<SwmSnippet path="config.def.h" line="22">

---

We define the color scheme using the Catppuccin Mocha palette:

```

static const char cat_base[]        = "#1e1e2e"; // catppuccin mocha base
static const char cat_surface1[]    = "#45475a"; // catppuccin mocha surface1
static const char cat_mantle[]      = "#181825"; // catppuccin mocha mantle
static const char cat_blue[]        = "#89b4fa"; // catppuccin mocha blue

static const char *colors[][3]      = {
	/*               fg            bg          border   */
	[SchemeNorm] = { cat_surface1, cat_mantle, cat_mantle   },
	[SchemeSel]  = { cat_blue,     cat_base,   cat_surface1 },
};
```

---

</SwmSnippet>

# Key definitions and commands

Key definitions and commands are crucial for user interaction. We define the modifier keys and key bindings for various actions.

<SwmSnippet path="config.def.h" line="91">

---

Modifier keys are defined for easier reference in key bindings:

```
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
```

---

</SwmSnippet>

<SwmSnippet path="config.def.h" line="101">

---

We define commands for launching applications and controlling volume:

```

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
```

---

</SwmSnippet>

<SwmSnippet path="config.def.h" line="123">

---

Key bindings for launching applications and controlling volume are set up:

```

	{ 0,              XF86XK_AudioMute,        spawn,          {.v = muteMus } },
	{ 0,              XF86XK_AudioMicMute,     spawn,          {.v = muteMic } },
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = volumeUp } },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = volumeDn } },

```

---

</SwmSnippet>

<SwmSnippet path="config.def.h" line="136">

---

```
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
```

---

</SwmSnippet>

<SwmSnippet path="config.def.h" line="147">

---

```
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
```

---

</SwmSnippet>

# Client and monitor management

Client and monitor management involves handling window properties and interactions between windows and monitors.

<SwmSnippet path="dwm.c" line="121">

---

We define the structure for managing monitor properties:

```
typedef struct Pertag Pertag;

struct Monitor {
	char ltsymbol[16];
	char monmark[16];
	float mfact;
	int nmaster;
	int num;
	int by;               /* bar geometry */
	int mx, my, mw, mh;   /* screen size */
	int wx, wy, ww, wh;   /* window area  */
	unsigned int borderpx;
	unsigned int seltags;
	unsigned int sellt;
	unsigned int tagset[2];
	int showbar, oldbar;
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="459">

---

We implement the function to attach a new client below the currently selected client:

```
void
attachBelow(Client *c)
{
	//If there is nothing on the monitor or the selected client is floating, attach as normal
	if(c->mon->sel == NULL || c->mon->sel->isfloating) {
        Client *at = nexttagged(c);
        if(!at) {
            attach(c);
            return;
            }
        c->next = at->next;
        at->next = c;
		return;
	}

	//Set the new client's next property to the same as the currently selected clients next
	c->next = c->mon->sel->next;
	//Set the currently selected clients next property to the new client
	c->mon->sel->next = c;

}
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="968">

---

We handle the focus change between monitors:

```
void
focusnthmon(const Arg *arg)
{
	Monitor *m;

	if (!mons->next)
		return;

	if ((m = numtomon(arg->i)) == selmon)
		return;
	unfocus(selmon->sel, 0);
	selmon = m;
	focus(NULL);
}

```

---

</SwmSnippet>

# Layout and gap management

Layout and gap management ensures windows are arranged correctly and gaps are applied as needed.

<SwmSnippet path="dwm.c" line="1735">

---

We define the function to set gaps between windows:

```
void
setgaps(const Arg *arg)
{
	switch(arg->i)
	{
		case GAP_TOGGLE:
			selmon->pertag->drawwithgaps[selmon->pertag->curtag] = !selmon->pertag->drawwithgaps[selmon->pertag->curtag];
			break;
		case GAP_RESET:
			if (selmon->pertag->curtag > 0)
				selmon->pertag->gappx[selmon->pertag->curtag] = gappx[selmon->pertag->curtag - 1 % LENGTH(gappx)];
			else
				selmon->pertag->gappx[0] = gappx[0];
			break;
		default:
			if (selmon->pertag->gappx[selmon->pertag->curtag] + arg->i < 0)
				selmon->pertag->gappx[selmon->pertag->curtag] = 0;
			else
				selmon->pertag->gappx[selmon->pertag->curtag] += arg->i;
	}
	arrange(selmon);
}
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="2010">

---

We implement the tiling layout with support for gaps:

```

void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}

	if (n <= 0)
		return;

	if (!m->pertag->drawwithgaps[m->pertag->curtag]) { /* draw with singularborders logic */
		if (n > m->nmaster)
			mw = m->nmaster ? m->ww * m->mfact : 0;
		else
			mw = m->ww;
		for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if (i < m->nmaster) {
				h = mfacts > 0 ? (m->wh - my) * (c->cfact / mfacts) : (m->wh - my) / (m->nmaster - i);
				resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
				if (my + HEIGHT(c) < m->wh)
					my += HEIGHT(c);
				mfacts -= c->cfact;
			} else {
				h = sfacts > 0 ? (m->wh - ty) * (c->cfact / sfacts) : (n > m->nmaster ? (m->wh - ty) / (n - m->nmaster) : m->wh - ty);
				resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
				if (ty + HEIGHT(c) < m->wh)
					ty += HEIGHT(c);
				sfacts -= c->cfact;
			}
		}
	} else { /* draw with fullgaps logic */
		if (n > m->nmaster)
			mw = m->nmaster ? m->ww * m->mfact : 0;
		else
			mw = m->ww - m->pertag->gappx[m->pertag->curtag];
		for (i = 0, my = ty = m->pertag->gappx[m->pertag->curtag], c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if (i < m->nmaster) {
				h = (MIN(n, m->nmaster) - i > 0) ? (m->wh - my) / (MIN(n, m->nmaster) - i) - m->pertag->gappx[m->pertag->curtag] : m->wh - my;
				resize(c, m->wx + m->pertag->gappx[m->pertag->curtag], m->wy + my, mw - (2*c->bw) - m->pertag->gappx[m->pertag->curtag], h - (2*c->bw), 0);
				if (my + HEIGHT(c) + m->pertag->gappx[m->pertag->curtag] < m->wh)
					my += HEIGHT(c) + m->pertag->gappx[m->pertag->curtag];
				mfacts -= c->cfact;
			} else {
				h = (n - i > 0) ? (m->wh - ty) / (n - i) - m->pertag->gappx[m->pertag->curtag] : m->wh - ty;
				resize(c, m->wx + mw + m->pertag->gappx[m->pertag->curtag], m->wy + ty, m->ww - mw - (2*c->bw) - 2*m->pertag->gappx[m->pertag->curtag], h - (2*c->bw), 0);
				if (ty + HEIGHT(c) + m->pertag->gappx[m->pertag->curtag] < m->wh)
					ty += HEIGHT(c) + m->pertag->gappx[m->pertag->curtag];
			}
		}
	}
}
```

---

</SwmSnippet>

# Event handling and focus management

Event handling and focus management are essential for responding to user actions and maintaining the correct focus on windows.

<SwmSnippet path="dwm.c" line="2069">

---

We handle the toggling of the bar visibility:

```

void
togglebar(const Arg *arg)
{
	selmon->showbar = selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;
	updatebarpos(selmon);
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx + sp, selmon->by + vp, selmon->ww - 2 * sp, bh);
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="2124">

---

We manage the focus change when switching views:

```

void
toggleview(const Arg *arg)
{
	unsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);
	int i;

	if (newtagset) {
		selmon->tagset[selmon->seltags] = newtagset;
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="2145">

---

```

		/* apply settings for this view */
		selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];
		selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
		selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
		selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
		selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];

		if (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])
			togglebar(NULL);

		focus(NULL);
		arrange(selmon);
	}
	updatecurrentdesktop();
```

---

</SwmSnippet>

<SwmSnippet path="dwm.c" line="2268">

---

We update the current desktop property to reflect the active tag:

```
void updatecurrentdesktop(void){
	long rawdata[] = { selmon->tagset[selmon->seltags] };
	int i=0;
    while(*rawdata >> (i+1)) {
        i++;
    }
	long data[] = { i };
	XChangeProperty(dpy, root, netatom[NetCurrentDesktop], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}
```

---

</SwmSnippet>

This concludes the walkthrough of the "throwaway branch with all applied patches" feature. The changes enhance the window manager's appearance, key bindings, client management, layout handling, and event response.

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBZHdtJTNBJTNBa2Vzb3I=" repo-name="dwm"><sup>Powered by [Swimm](https://app.swimm.io/)</sup></SwmMeta>
