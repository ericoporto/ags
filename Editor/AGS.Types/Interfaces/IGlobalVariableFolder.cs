using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AGS.Types
{
    /// <summary>
    /// Provides access to global variables and their folders.
    /// </summary>
    public interface IGlobalVariableFolder
    {
        /// <summary>
        /// The name of this global variable folder.
        /// </summary>
        string Name { get; set; }

        /// <summary>
        /// Sub-folders contained within this folder.
        /// </summary>
        IList<IGlobalVariableFolder> SubFolders { get; }

        /// <summary>
        /// Global variables in this folder.
        /// </summary>
        IList<GlobalVariable> GlobalVariables { get; }

        /// <summary>
        /// Returns total number of global variables found in the current folder and sub-folders.
        /// </summary>
        int CountGlobalVariablesInAllSubFolders();

        /// <summary>
        /// Assembles a list of all the global variables in the current folder and sub-folders
        /// </summary>
        /// <returns>
        /// Returns an IList with all global variables from this folder and sub-folders
        /// </returns>
        IList<GlobalVariable> GetAllGlobalVariablesFromAllSubFolders();
    }
}
