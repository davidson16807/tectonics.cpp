# CODE STYLE:

| entity              | convention |
| ------------------- | ----------- |
| names in general    | use whole words unless otherwise indicated,   |
|                     | do not use acronyms or abbreviation unless it is already used by layman (e.g. `id` or `gui`, see `valid-abbreviations.md` for a complete list),  |
|                     | or already adopted by a standard or 3rd party dependency (e.g. `std`, `bvec3`, `mat2`, etc.)  |
| namespaces          | single lower case short words, e.g. `rock` or `gene`,   |
|                     | singular, usually implying a genitive case, adjective, or determiner,  |
|                     | avoid using `nested::name::spaces` unless strictly needed for disambiguation  |
| directories         | single lower case short words, e.g. `index` or `test`,  |
|                     | singular, prefer using `nested/sub/folders/` to group related functionality (e.g. `index/series/`, `field/noise/`)  |
|                     | or to bring attention to requirements for extended functionality (e.g. `math/glm/`, `index/si/`)  |
|                     | folders should be ordered so that more common dependencies appear last (e.g. `dymaxion/buffer`, not `buffer/dymaxion`⁰) |
| file names          | If the file pertains to a specific class or namespace, then use the name for that class/namespace, otherwise use   |
|                     | `SCREAM_CASE` for standard or high level documentation (e.g. `README.md`, `GLOSSARY.tsv`, `CATEGORY.png`),  |
|                     | `lower_case` for code files, and  |
|                     | `kebab-case` for non-code files  |
| classes             | `PascalCase` , err toward verbosity, use `Titlecase` for acronyms e.g. `Id`, `Si`, `Gui`, `Lod`  |
| data structures     | `snake_case` if trying to encourage treating the structure like a primitive, if so brevity is allowed,  |
|                     | `PascalCase` for all other circumstances, and err toward verbosity  |
| "class categories"  | `PascalCase`, do not suffix with `*er`, `*tor`, etc.¹, consider using the plural, or suffixing with `*ing`, `*tion`, etc.   |
| "curry classes"     | `PascalCase`, do not suffix with `*er`, `*tor`, etc.¹, consider using the plural, or suffixing with `*ing`, `*tion`, etc.   |
| "make_* functions"  | `snake_case`, copy the name of the class/structure being made, do not prefix with `make_*`  |
| macros              | `SCREAM_CASE`, err toward verbosity, include the namespace it pertains to, and limit its scope to a single file if possible (using `#undef`)  |
| global variables    | `SCREAM_CASE`, err toward verbosity  |
| impure methods      | `camelCase`², multiple words, brevity is allowed³, use standard prefixes where applicable (see `doc/function-prefixes.tsv` for details)  |
| impure functions    | `camelCase`², multiple words, err toward verbosity, use standard prefixes where applicable (see `doc/function-prefixes.tsv` for details)  |
| regular methods     | `snake_case`, brevity is allowed³, use standard prefixes where applicable (see `doc/function-prefixes.tsv` for details)  |
| regular functions   | `snake_case`, err toward verbosity, use standard prefixes where applicable (see `doc/function-prefixes.tsv` for details)  |
| parameters          | `snake_case`, err toward verbosity  |
| private variables   | `snake_case`, err toward verbosity  |
| local variables     | `snake_case`, brevity is allowed³  |
| booleans            | `snake_case`, prefix with `is_*`, `has_*`, `can_*`, etc.  |
| boolean vectors     | `snake_case`, prefix with `are_*`, `have_*`, `can_*`, etc.   |
| rasters             | same rules as if it were the underlying `value_type`  |
| ids                 | single lowercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above, standard placeholders are `i`,`j`,`k`)  |
| scalars             | single lowercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above, standard placeholders are `a`,`b`,`c`, `x`,`y`,`z`)  |
| function            | single lowercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above, standard placeholders are `f`,`g`,`h`)  |
| vectors             | single uppercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above, standard placeholders are `V`,`U`,`W`)  |
| matrices            | single uppercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above, standard placeholders are `A`,`B`,`C`)  |
| products            | concatenated names, if brevity or abstraction demands it⁵, otherwise determined by rules above  |
| other operators     | underscore delimited names, if brevity or abstraction demands it⁵, otherwise determined by rules above  |
| template parameters | single uppercase letters if brevity or abstraction demands it⁵, or to conform with 3rd party libraries like `glm`,   |
|                     | otherwise same rules as for data structures, do not prefix with `T` if so⁴.  |
|                     | Standard placeholders are  `T1`, `T2`, etc. for types, `R` for scalars, `Z` for ints, and `N` for uints.  |
| constants           | determined by rules above⁶  |

# NOTES:

⁰ Developers will more easily recognize common dependencies like `buffer`, 
  so nesting them deep like `dymaxion/buffer` is more likely to jog their memory,
  whereas finding a less common dependency nested in a common one like `buffer/dymaxion` will appear disordered and out-of-place.

¹ `*er`/`*tor` will be mistaken as focusing on an object-oriented approach, which is not what we're going for.
  The important thing to remind is not that the class ought to be something with agency that does things,
  but that it represents intercomposable, conceptually pure functions that together define some subject matter.

² Does not apply to conceptually pure functions, only functions that are impure by even a weak definition.
  The idea is that we need some way to draw attention to dangerous things⁷.
  Most of the object oriented code that's out in the wild is already rife with side effects, 
  so sticking with `camelCase` conventions will allow 3rd party code to remain consistent with our own conventions,
  while still allowing us to warn ourselves or others that the code they're calling may not be pure. 

³ Methods already have a lot of context built up from the fact that their classes are being used,
  and following our design approach typically results in sparse classes with methods that reinforce each other,
  so some familiarity with methods and their names should be expected.
  Aside from methods, most things that face an API user must be verbose, like parameters or function names.
  Private variables are still used in a scope that's broad enough to be treated just like anything else.
  Local variables are used frequently and are the only thing whose scope is limited enough to permit brevity.

⁴ This was recommend in a C++ lightning talk, whose title I can no longer remember.
  Template parameters behave just like regular parameters so they are low risk,
  and we want to reduce noise, so we only draw attention to things that are dangerous⁷.
  Dropping the prefix has resulted in some dramatic improvements to readability without sacraficing understanding,
  see various grid classes like `dymaxion::Grid.hpp` for examples of how this works well.

⁵ There absolutely are times where this is needed. For examples where brevity is needed, see `unit/si.hpp`,
  or the code that's responsible for atmospheric scattering in tectonics.js.
  For examples where abstraction is needed, see code for geometric intersections in tectonics.glsl.
  A lookup table should be provided in comments if abbreviation is done for brevity.
  Subscripting is okay (e.g. `Tout` for output temperature) but not multiletter abbreviation (e.g. `tempout`).
  Multiletter abbreviation is a holdout from the days of FORTRAN and global variables,
  when there were more variables in a local scope than letters to describe them.
  If you really need abbreviation that badly, then go all the way.
  Alternatively, if there are so many local variables that you need multiletter abbreviation, 
  then your local scope is too big.

⁶ Constants have no particular convention, their case is determined by other properties. 
  Constants place a strict compile-time guarantee that a variable will not be modified during runtime.
  We can rely on these guarantees to reason with code, so they are helpful.
  We only draw attention to things that are dangerous⁷.
  Nonconstant globals on the other hand would require `SCREAM_CASE`, 
  but we forbid their use so this should not occur in practice.
  Globals introduce side effects to code and make the code difficult to reason with.
  Constants are good, globals are evil. 

⁷ no such thing as an "everything's okay alarm"