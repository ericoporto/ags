using System;
using NUnit.Framework;

namespace AGS.Editor
{
    [TestFixture]
    public class IncludeUtilsTests
    {
        string[] exampleGameFiles = new string[] {
            "acsprset.spr",
            "AGSFNT0.WFN",
            "AGSFNT1.WFN",
            "AGSFNT2.WFN",
            "AudioCache",
            "Compiled",
            "Compiled/Data",
            "Game.agf",
            "Game.agf.bak",
            "Game.agf.user",
            "GlobalScript.asc",
            "GlobalScript.ash",
            "KeyboardMovement.asc",
            "KeyboardMovement.ash",
            "KeyboardMovement.txt",
            "room1.asc",
            "room1.crm",
            "Speech",
            "sprindex.dat",
            "Sprites",
            "Sprites/bluecup0.png",
            "Sprites/Defaults",
            "Sprites/Defaults/Cursors",
            "Sprites/Defaults/Cursors/cursor_interact.png",
            "Sprites/Defaults/ico_bluecup.png",
            "Sprites/Defaults/ico_key.png",
            "Sprites/Defaults/ico_x.png",
            "Sprites/Defaults/UI",
            "Sprites/Defaults/UI/background_inventory.png",
            "Sprites/Defaults/Verbs",
            "Sprites/Defaults/Verbs/btn_arrowdown_normal.png",
            "Sprites/Defaults/Verbs/btn_arrowdown_over.png",
            "template.files",
            "template.ico",
            "template.txt",
            "_Debug",
            "_Debug/acsetup.cfg",
            "_Debug/SDL2.dll",
            "_Debug/Sierra-style.exe"
        };

        [Test]
        public void CreatePatternList_ParsesPatternsCorrectly()
        {
            string patternText = @"
*.asc
!ignoreme.asc
folder/*
";

            var patterns = IncludeUtils.CreatePatternList(patternText);

            Assert.That(patterns.Length, Is.EqualTo(3));
            Assert.That(patterns[0].Type, Is.EqualTo(IncludeUtils.PatternType.Include));
            Assert.That(patterns[1].Type, Is.EqualTo(IncludeUtils.PatternType.Exclude));
            Assert.That(patterns[2].Type, Is.EqualTo(IncludeUtils.PatternType.Include));

            Assert.That(patterns[0].OriginalPattern, Is.EqualTo("*.asc"));
            Assert.That(patterns[1].OriginalPattern, Is.EqualTo("ignoreme.asc"));
            Assert.That(patterns[2].OriginalPattern, Is.EqualTo("folder/*"));
        }

        [Test]
        public void FilterItemList_CaseInsensitiveMatching()
        {
            string patternText = @"
*.TXT
";
            var patterns = IncludeUtils.CreatePatternList(patternText, IncludeUtils.MatchOption.CaseInsensitive);

            string[] items = new string[] {
                "file1.txt",
                "file2.TXT",
                "file3.TxT"
            };

            var result = IncludeUtils.FilterItemList(items, patterns, IncludeUtils.MatchOption.CaseInsensitive);

            Assert.That(result.Length, Is.EqualTo(3));
        }

        [Test]
        public void FilterItemList_PatternOrderMatters()
        {
            string patternText = @"
*.txt
!exclude.txt
*.txt
";
            var patterns = IncludeUtils.CreatePatternList(patternText);

            string[] items = new string[] {
                "file.txt",
                "exclude.txt"
            };

            var result = IncludeUtils.FilterItemList(items, patterns);

            Assert.That(result, Does.Contain("file.txt"));
            Assert.That(result, Does.Contain("exclude.txt"));
        }

        [Test]
        public void EmptyOrCommentLines_AreIgnored()
        {
            string patternText = @"
# This is a comment
*.dat

!ignore.dat
";

            var patterns = IncludeUtils.CreatePatternList(patternText);

            Assert.That(patterns.Length, Is.EqualTo(2));
            Assert.That(patterns[0].OriginalPattern, Is.EqualTo("*.dat"));
            Assert.That(patterns[1].OriginalPattern, Is.EqualTo("ignore.dat"));
        }

        [Test]
        public void FilterItemList_ExampleTemplate()
        {
            string patternText = @"
*.WFN
Game.agf
*.asc
*.ash
*.crm
Sprites/
*.txt
template.ico
";
            var patterns = IncludeUtils.CreatePatternList(patternText);

            var result = IncludeUtils.FilterItemList(exampleGameFiles, patterns);

            Assert.That(result, Does.Contain("AGSFNT0.WFN"));
            Assert.That(result, Does.Contain("AGSFNT1.WFN"));
            Assert.That(result, Does.Contain("AGSFNT2.WFN"));
            Assert.That(result, Does.Contain("Game.agf"));
            Assert.That(result, Does.Contain("GlobalScript.asc"));
            Assert.That(result, Does.Contain("GlobalScript.ash"));
            Assert.That(result, Does.Contain("KeyboardMovement.asc"));
            Assert.That(result, Does.Contain("KeyboardMovement.ash"));
            Assert.That(result, Does.Contain("KeyboardMovement.txt"));
            Assert.That(result, Does.Contain("room1.asc"));
            Assert.That(result, Does.Contain("room1.crm"));
            Assert.That(result, Does.Contain("Sprites/bluecup0.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults"));
            Assert.That(result, Does.Contain("Sprites/Defaults/Cursors"));
            Assert.That(result, Does.Contain("Sprites/Defaults/Cursors/cursor_interact.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/ico_bluecup.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/ico_key.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/ico_x.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/UI"));
            Assert.That(result, Does.Contain("Sprites/Defaults/UI/background_inventory.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/Verbs"));
            Assert.That(result, Does.Contain("Sprites/Defaults/Verbs/btn_arrowdown_normal.png"));
            Assert.That(result, Does.Contain("Sprites/Defaults/Verbs/btn_arrowdown_over.png"));
            Assert.That(result, Does.Contain("template.ico"));
            Assert.That(result, Does.Contain("template.txt"));
            Assert.That(result, Does.Not.Contain("Game.agf.bak"));
            Assert.That(result, Does.Not.Contain("Game.agf.user"));
            Assert.That(result.Length, Is.EqualTo(25));
        }
    }
}
