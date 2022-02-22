using AGS.Types;
using NUnit.Framework;

namespace AGS.Editor
{
    [TestFixture]
    public class AutoCompleteTests
    {
        private static Script CachedAutoCompletedScriptFromCode(string scriptCode)
        {
            Script script = new Script("_test", scriptCode, false);
            AutoComplete.ConstructCache(script);
            return script;
        }

        [SetUp]
        public void SetUp()
        {
        }

        [Test]
        public void CheckAutoCompleteVariables()
        {
            string scriptCode = $@"
int a;
";
            Script scriptToTest = CachedAutoCompletedScriptFromCode(scriptCode);

            Assert.That(scriptToTest.AutoCompleteData.Variables.Count, Is.EqualTo(1));            
        }
    }
}
