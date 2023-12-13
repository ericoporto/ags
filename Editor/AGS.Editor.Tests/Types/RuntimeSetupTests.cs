using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace AGS.Types
{
    [TestFixture]
    public class RuntimeSetupTests
    {
        private Settings _dummySettings;

        [SetUp]
        public void SetUp()
        {
            _dummySettings = new Settings();
        }

        [Test]
        public void Constructor_InitializesProperties()
        {
            var runtimeSetup = new RuntimeSetup(_dummySettings);

            Assert.IsNotNull(runtimeSetup);
            Assert.AreEqual(GraphicsDriver.D3D9, runtimeSetup.GraphicsDriver);
            Assert.IsFalse(runtimeSetup.Windowed);
            Assert.AreEqual(GameScaling.ProportionalStretch, runtimeSetup.FullscreenGameScaling);
        }

        [Test]
        public void SetDefaults_SetsDefaultValues()
        {
            var runtimeSetup = new RuntimeSetup(_dummySettings);

            runtimeSetup.SetDefaults();

            Assert.AreEqual(GraphicsDriver.D3D9, runtimeSetup.GraphicsDriver);
            Assert.IsFalse(runtimeSetup.Windowed);
            Assert.AreEqual(GameScaling.ProportionalStretch, runtimeSetup.FullscreenGameScaling);
        }

        [Test]
        public void GraphicsDriver_Setter_UpdatesGraphicsFilter()
        {
            var runtimeSetup = new RuntimeSetup(_dummySettings);

            runtimeSetup.GraphicsDriver = GraphicsDriver.OpenGL;

            Assert.IsNotNull(runtimeSetup.GraphicsFilter);
            Assert.AreEqual(GraphicsDriver.OpenGL, runtimeSetup.GraphicsDriver);
        }

        [Test]
        public void UseCustomSavePath_Setter_UpdatesCustomSavePath()
        {
            var runtimeSetup = new RuntimeSetup(_dummySettings);

            runtimeSetup.UseCustomSavePath = true;
            runtimeSetup.CustomSavePath = "CustomPath";

            Assert.IsTrue(runtimeSetup.UseCustomSavePath);
            Assert.AreEqual("CustomPath", runtimeSetup.CustomSavePath);
        }

        [Test]
        public void Clone_ShouldMatchOriginal()
        {
            Settings gameSettings = new Settings();
            RuntimeSetup originalSetup = new RuntimeSetup(gameSettings)
            {
                GraphicsDriver = GraphicsDriver.OpenGL,
                Windowed = true,
                FullscreenDesktop = true,
                FullscreenGameScaling = GameScaling.MaxInteger,
                GameScaling = GameScaling.Integer,
                GameScalingMultiplier = 2,
                GraphicsFilter = "linear",
                VSync = true,
                AAScaledSprites = true,
                RenderAtScreenResolution = true,
                Rotation = ScreenRotationMode.Landscape,
                DigitalSound = RuntimeAudioDriver.Default,
                UseVoicePack = false,
                Translation = "Portuguese",
                AutoLockMouse = true,
                MouseSpeed = 1.5f,
                TouchToMouseEmulation = TouchToMouseEmulationType.TwoFingers,
                SpriteCacheSize = 256,
                TextureCacheSize = 256,
                SoundCacheSize = 64,
                UseCustomSavePath = true,
                CustomSavePath = "CustomSaves",
                UseCustomAppDataPath = true,
                CustomAppDataPath = "CustomAppData",
                TitleText = "Custom Setup",
                ShowFPS = true
            };

            RuntimeSetup cloneSetup = originalSetup.Clone();

            Assert.That(cloneSetup, Is.Not.SameAs(originalSetup), "Clone should be a different instance");
            Assert.That(cloneSetup.GraphicsDriver, Is.EqualTo(originalSetup.GraphicsDriver), "Clone should match the original (GraphicsDriver)");
            Assert.That(cloneSetup.Windowed, Is.EqualTo(originalSetup.Windowed), "Clone should match the original (Windowed)");
            Assert.That(cloneSetup.FullscreenDesktop, Is.EqualTo(originalSetup.FullscreenDesktop), "Clone should match the original (FullscreenDesktop)");
            Assert.That(cloneSetup.FullscreenGameScaling, Is.EqualTo(originalSetup.FullscreenGameScaling), "Clone should match the original (FullscreenGameScaling)");
            Assert.That(cloneSetup.GameScaling, Is.EqualTo(originalSetup.GameScaling), "Clone should match the original (GameScaling)");
            Assert.That(cloneSetup.GameScalingMultiplier, Is.EqualTo(originalSetup.GameScalingMultiplier), "Clone should match the original (GameScalingMultiplier)");
            Assert.That(cloneSetup.GraphicsFilter, Is.EqualTo(originalSetup.GraphicsFilter), "Clone should match the original(GraphicsFilter)");
            Assert.That(cloneSetup.VSync, Is.EqualTo(originalSetup.VSync), "Clone should match the original (VSync)");
            Assert.That(cloneSetup.AAScaledSprites, Is.EqualTo(originalSetup.AAScaledSprites), "Clone should match the original (AAScaledSprites)");
            Assert.That(cloneSetup.RenderAtScreenResolution, Is.EqualTo(originalSetup.RenderAtScreenResolution), "Clone should match the original (RenderAtScreenResolution)");
            Assert.That(cloneSetup.Rotation, Is.EqualTo(originalSetup.Rotation), "Clone should match the original (Rotation)");
            Assert.That(cloneSetup.DigitalSound, Is.EqualTo(originalSetup.DigitalSound), "Clone should match the original (DigitalSound)");
            Assert.That(cloneSetup.UseVoicePack, Is.EqualTo(originalSetup.UseVoicePack), "Clone should match the original (UseVoicePack)");
            Assert.That(cloneSetup.Translation, Is.EqualTo(originalSetup.Translation), "Clone should match the original (Translation)");
            Assert.That(cloneSetup.AutoLockMouse, Is.EqualTo(originalSetup.AutoLockMouse), "Clone should match the original (AutoLockMouse)");
            Assert.That(cloneSetup.MouseSpeed, Is.EqualTo(originalSetup.MouseSpeed), "Clone should match the original (MouseSpeed)");
            Assert.That(cloneSetup.TouchToMouseEmulation, Is.EqualTo(originalSetup.TouchToMouseEmulation), "Clone should match the original (TouchToMouseEmulation)");
            Assert.That(cloneSetup.SpriteCacheSize, Is.EqualTo(originalSetup.SpriteCacheSize), "Clone should match the original (SpriteCacheSize)");
            Assert.That(cloneSetup.TextureCacheSize, Is.EqualTo(originalSetup.TextureCacheSize), "Clone should match the original (TextureCacheSize)");
            Assert.That(cloneSetup.SoundCacheSize, Is.EqualTo(originalSetup.SoundCacheSize), "Clone should match the original (SoundCacheSize)");
            Assert.That(cloneSetup.UseCustomSavePath, Is.EqualTo(originalSetup.UseCustomSavePath), "Clone should match the original (UseCustomSavePath)");
            Assert.That(cloneSetup.CustomSavePath, Is.EqualTo(originalSetup.CustomSavePath), "Clone should match the original (CustomSavePath)");
            Assert.That(cloneSetup.UseCustomAppDataPath, Is.EqualTo(originalSetup.UseCustomAppDataPath), "Clone should match the original");
            Assert.That(cloneSetup.CustomAppDataPath, Is.EqualTo(originalSetup.CustomAppDataPath), "Clone should match the original (CustomAppDataPath)");
            Assert.That(cloneSetup.TitleText, Is.EqualTo(originalSetup.TitleText), "Clone should match the original (TitleText)");
            Assert.That(cloneSetup.ShowFPS, Is.EqualTo(originalSetup.ShowFPS), "Clone should match the original (ShowFPS)");
        }
    }
}
