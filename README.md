# Guess The Number - Flipper Zero

> An enhanced number guessing game with progressive feedback, score tracking, and immersive multi-sensory experience.

## ✨ Features

- **Progressive Hint System** - Distance-based feedback (very close, close, far)
- **Score Tracking** - Personal best score persistence across games
- **Multi-Sensory Feedback** - Dynamic LED colors, haptic patterns, and sound effects
- **Intuitive Controls** - Fine (+/-1) and coarse (+/-10) adjustment modes
- **Professional UI** - Centered layout with visual emphasis and clear typography

## 🎮 Gameplay

Guess the secret number (0-99) in the fewest attempts possible. The game provides intelligent feedback based on your proximity to the target, with enhanced audio and visual cues.

**Controls:**

- `↑/↓` Fine adjust (+/-1)
- `←/→` Quick adjust (+/-10)
- `OK` Submit guess
- `BACK` Reset game (hold to exit)

## 🚀 Installation

```bash
git clone https://github.com/kWAYTV/guess-the-number-fz
cd guess-the-number-fz
ufbt update --index-url=https://up.momentum-fw.dev/firmware/directory.json --hw-target=f7 --channel=dev
ufbt
```

Deploy the generated `.fap` file to your Flipper Zero's `apps/Games` directory.

## 🛠️ Development

Built with [Flipper Zero SDK](https://docs.flipper.io) using [leedave's boilerplate](https://github.com/leedave/flipper-zero-fap-boilerplate).

**Build Requirements:**

- [uFBT](https://github.com/flipperdevices/flipperzero-ufbt)
- Flipper Zero firmware API v86.0+

## 📄 License

MIT License - see [LICENSE](LICENSE) for details.

---

**Target:** 7 | **API:** 86.0 | **Category:** Games
