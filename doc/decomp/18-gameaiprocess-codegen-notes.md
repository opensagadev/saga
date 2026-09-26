# GameAIProcess code-generation notes

These are measured patterns from matching `_Z13GameAIProcessv` in
`src/legoapi/items/objects/gameobjects.cpp`. The reference is `res/libTTapp.so`;
the current code is the Android x86 target build. Scores below are **whole
symbol** match percentages from the linked-GOT `objdiff-cli` fork on branch
`codex/i386-linked-got` (`08fd60d3`). Each comparison used the same build and
diff settings. They are observations for this function, not universal GCC 4.7
rules.

Reproduce a focused comparison:

```text
bazelisk build --config=target //src:saga_target
objdiff-cli diff -o gameaiprocess.json -1 res/libTTapp.so -2 bazel-bin/src/libTTapp.so _Z13GameAIProcessv
python scripts/objdiff-cli.py --no-color -C 0 -t bazel-bin/src/libTTapp.so _Z13GameAIProcessv
```

The direct `objdiff-cli` command is useful for the whole-symbol score; the
repository wrapper shows aligned instruction differences. Use the linked-GOT
fork for this binary rather than an unrelated release of the CLI.

## Repeated patterns

| Source choice | Measured result | Matching lesson |
|---|---|---|
| Precompute `-lateral.x` and `-lateral.z` before the first `GameRayCast`, then assign them to the reverse ray after the call | 25.989584% to 26.518679% | Expressions across a non-inlined call affect instruction scheduling and live registers. The original computes the negated components before the first ray cast. Reusing those values for a later `wall_direction` assignment instead lowered the score to 24.239584%. Compare each use separately. |
| Use `i32` for local decision flags and a byte-sized bitwise test for the pair | `i32 defend` raised 22.723420% to 23.102371%; the byte test raised 24.934626% to 25.084412% in one branch arrangement | Local width and the form of a combined test affect register and branch selection. Neither form is a universal improvement: changing both flags to `i32` in an earlier arrangement regressed. |
| Place the equality check between the two collision-priority updates | 25.084412% to 25.989584% | Equivalent-looking condition order changes GCC's control-flow graph. Reversing a comparison from `a > b` to `b < a` later lowered 25.989584% to 24.268318%. Preserve the target's tested order. |
| Recheck the antinode timer clamp against the actual SSE predicate | The former `if (!(timer >= 0.0f))` scored 46.495330% on the later layout; correcting it to `if (timer < 0.0f)` scores 46.447918% at the same 14,197-byte size | The target uses `cmpnltss` and `andps`, retaining the timer on an unordered comparison. The former source clears it. The corrected source emits `maxss`, which retains a quiet NaN in the timer source; a slightly higher whole-symbol score did not mean the former source was semantically right. |
| Advance an `APIOBJECT **neutral_cursor` when collecting neutral objects | 26.518679% to 26.663433% | A pointer walk changed the register allocation favorably here. At that earlier stage, a goodies cursor alone also helped, but combining it with the neutral cursor fell to 26.127874%. Whole-function scores are not additive; the later layout reverses this result. |
| Cache `object->pad_gamepad` for the two consecutive first-loop button masks | 26.663433% to 26.834412% | The original loads the pad pointer once before updating both fields. The uncached source reloaded it between stores. Keeping a local pointer recovered that instruction shape in this block. |
| Mark only the first-loop sign-bit touch test likely, while leaving the `TouchControlsActive` test unhinted | 37.798850% to 37.907690%; size 14,051 to 14,083 bytes | The target tests the sign bit and uses `jns` to skip the touch path, letting the touch body fall through. The unhinted build used `js` into a distant touch block. Hinting the whole combined `&&` condition had earlier regressed and did not flip this branch. Apply branch hints to the exact subcondition whose layout matters. |
| Split the stuck-toggle timer choice into explicit zero and quarter-second stores | 37.911278% to 37.935345%; size 14,083 to 14,113 bytes | The target writes literal `0` and `0x3e800000` in separate branches. The ternary source made GCC select a float in a register and share one store. The explicit branches recovered both literal store forms and improved whole-symbol alignment. This applies to the stuck-toggle site; the later route-toggle site retains its shared assignment. |
| Preserve the route timer's zero store with a zero-instruction `+m` constraint on that field | 37.935345% to 38.002514%; size 14,113 to 14,117 bytes | The target writes `0` to `object+0xda4` when starting a route, then reloads it for the frame-time subtraction. GCC otherwise eliminates the store. A field-only read/write constraint keeps that operation while disturbing less code than a whole-memory barrier or volatile store, both of which regressed. |
| Change that route timer constraint to input-only `m` on the later layout | 40.904095% to 40.916310%; size 14,091 to 14,083 bytes | Both operands preserve the zero store. Input-only `m` lets GCC keep zero in `xmm1` and emit the target's `movaps xmm0, xmm1` before subtracting frame time; read/write `+m` forced a reload from `object+0xda4`. Constraint direction can alter value propagation even when the empty asm emits no instruction. |
| Carry the raw `field_0xf00` byte through the main loop, test its process bit, and shift it for the `AISysProcessCharacter` argument | 40.916310% to 41.043102%; size 14,083 to 14,099 bytes | The old source turned bit 3 into a boolean early; the target carries the byte in `edx`, tests bit 3, and derives a 0/1 call argument with a shift and mask. Both `i32` and `u8` declarations for the raw local compiled identically. Removing the now counterproductive field-scoped `+m` barrier raised the later raw-byte layout to 41.136494% at the same size. |
| Keep the goody cursor in the player scan but index its two main-loop collection stores | 41.136494% to 41.221622%; after restoring the `0x570` frame, 41.224857%; size 14,099 bytes | The main loop no longer needs to carry that cursor after the player scan. Its removal from only those two sites shrinks the frame to `0x560`; increasing the existing stack placeholder from 32 to 48 bytes restores the target's frame and four pointer-array offsets. Removing the cursor from the earlier scan as well lowers the score, so use different store forms in the two phases. |
| With main-loop goodies indexed, advance a baddy pointer and index main-loop neutrals | 41.224857% to 41.234196% after a 32-byte placeholder, then to 41.309628% with the neutral store and a 48-byte placeholder; size 14,099 to 14,131 bytes | The baddy cursor alone initially grows the frame to `0x580`; shrinking padding restores `0x570`. Indexing neutrals then shrinks it to `0x560`, so padding returns to 48 bytes. These combined choices improve the register/stack layout even though indexing neutrals alone had regressed on the prior layout. Keep the earlier player-scan neutral cursor. |
| Hint the outer unfixed-first collision path likely, then leave an empty marker after its first helper call | 41.309628% to 41.357040% with the hint, then to 41.664154% with the marker; size 14,131 to 14,166 bytes | The pair prevents GCC from merging two negated helper-call tails and keeps all three emitted calls in the cold collision region. The marker alone on the previous layout put one call in the hot region and regressed. The target still differs in call placement, pointer registers, and vector stack slot; test this pair as one layout choice. |
| Move that empty marker from the first negated call to the non-negated second call | 41.664154% to 41.723778%; size stays 14,166 bytes | The compiler still emits three helper-call sites in the cold tail, but their placement and surrounding spills improve. A marker after only the third call leaves two sites; markers after both the first and second collapse back to two. The exact branch carrying the otherwise empty marker changes cross-jumping. |
| Mark only the final player-scan `any_uncovered == 0` condition unlikely | 41.723778% to 42.579020%; size 14,166 to 14,175 bytes | This changes the scan-exit block layout while preserving the current accumulator and stack allocation. Marking it likely scored 41.405533% (14,150 bytes). Apply the hint to this subcondition rather than the entire conjunction. |
| Test `field_0xf00` directly from memory in the early path-update branch, while keeping its loaded byte live through that branch | 42.579020% to 42.639730%; size stays 14,175 bytes | The target uses `test byte ptr [object+0xf00], 8` at that site. Replacing the cached test alone recovered the local instruction but fell to 42.225933% and 14,127 bytes. An empty input-register use of the cached byte just before the direct test restores the wider register lifetime and raises the whole-symbol match. The same marker immediately after the early byte load is redundant. |
| Use the player-scan goody cursor for both main-loop goody stores, then reduce the stack placeholder to 32 bytes | 42.639730% to 42.687500% with the 48-byte placeholder, then 42.690730% with a restored `0x570` frame; size stays 14,175 bytes | The target's first `0x400` collection path advances a pointer, while the indexed source used a scaled store. Both main-loop writes must advance the cursor to preserve their ordering when the paths alternate. Switching only one path briefly scored 42.662716%, but that source could overwrite a prior indexed entry and was discarded as incorrect. With both paths using the cursor, shrinking the existing placeholder restores the target frame size and list-array offsets. |
| Mark the post-`AISysProcessCharacter` non-player path likely | 42.690730% to 43.481323%; reducing the stack placeholder from 32 to 16 bytes then gave 43.484554% with the target `0x570` frame; size 14,175 to 14,197 bytes | This changes the broader block layout without moving the wall-shuffle body to its target position. Marking the same path unlikely scored 36.119614% (14,102 bytes) and pushed wall-shuffle to the far tail. A likelihood hint on this outer branch has a much larger effect than hints on the nested wall-shuffle guard. |
| Mark only the stuck-path `path_connection_state == 0` subcondition unlikely | 43.484554% to 43.532690%; size 14,197 to 14,186 bytes, `0x570` frame | The target's wall-shuffle body still sits at a different position, but the neighboring stuck-path blocks align slightly better. Marking the same state check likely scored 43.090157% (14,212 bytes). Keep the hint on this first subcondition, rather than assuming the whole conjunction has the same effect. |
| Express the first cover scan as positive `all_under_cover`, while keeping the final unlikely decision hint | 43.532690% to 43.937500%; size stays 14,186 bytes and frame stays `0x570` | This equivalent accumulator polarity had regressed on earlier layouts. It still emits zero `cmovne` and eight `cmove` instructions, so the gain is in the wider branch and register alignment rather than recovery of the target's cover-update opcode. Recheck prior dead ends after substantial layout changes. |
| Set the positive cover accumulator to zero with an explicit `if` when the model flag is absent | 43.937500% to 43.979527%; size stays 14,186 bytes | The condition compiles into the same broad scan shape with eight `cmove` instructions, but local scheduling improves the whole-symbol alignment. A bitwise `&=` update instead scored 42.669900% (14,130 bytes) and left only one `cmove`; the explicit conditional assignment is retained. |
| Mark first-loop alert-target timer expiry unlikely | 43.979527% to 44.953304%; size 14,186 to 14,218 bytes, `0x570` frame | The target subtracts frame time, compares the result with zero, and takes a long `jae` to the reset block. Without a hint, GCC kept the reset inline and used `jb` to skip it. A hint on only the expiry test restores the target's cold reset branch and the following loop-latch shape. |
| Mark the wall-avoidance distance-above-`ai_moveradius` scale path likely | 44.953304% to 45.610270%; size 14,218 to 14,206 bytes, `0x570` frame | The target uses a short `jbe` to skip scaling, then keeps the divide, doubled scale, and `NuVecScale` call inline. Without this hint, GCC used a long `ja` into a distant scale block. The local branch now has the right direction and neighboring instruction sequence, though its XMM and stack operands still differ. |
| Express the positive cover update as `flag ? all_under_cover : 0` | 45.610270% to 45.653020%; size stays 14,206 bytes, `0x570` frame | The ternary is equivalent to the prior explicit zeroing branch and the reversed `flag_missing ? 0 : all_under_cover` polarity compiles identically. It still emits eight `cmove` and no `cmovne`; the small gain comes from surrounding instruction alignment. This retest reverses the earlier ranking at 37.798850%. |
| Require the first cover scan's `VADER_ADATA`, `party_under_cover`, `active_neutral_count`, and `FreePlay` GOT addresses in target order, then read the cover global through an input-only memory operand | 45.653020% to 45.764366% from the address ordering, then 45.778378% from the cover read; size stays 14,206 bytes, `0x570` frame | The four input-register operands recover the target's early GOT-load order without emitted asm instructions. The input-only memory operand changes scheduling around the two zero stores. A read/write operand instead scores 45.493176%; the direction of an empty asm constraint still matters. |
| Mark the collision-pair check where both antinode timers exceed `antinode_time` likely | 45.778378% to 46.261494%; size 14,206 to 14,192 bytes, `0x570` frame | This changes the long collision block layout while keeping both timer checks and their fallbacks intact. Marking the same conjunction unlikely scored 45.164513%. The aligned first timer branch still differs from the target (`ja` versus `jbe`), so the gain is broader alignment rather than an exact local branch match. |
| Mark the `JEDI_B_LDATA` test in the Jango hover case unlikely after the timer hint | 46.261494% to 46.320040%; size 14,192 to 14,186 bytes, `0x570` frame | This isolated subcondition steers block ordering without changing the rest of the conjunction. Tested alone, it scored 45.853090% against 45.778378%; combined gain is smaller. Marking the level test likely scored 44.343750% on the earlier baseline. The Jango body still resides far from its target aligned position, so keep measuring whole-symbol effects. |
| Mark the final per-object `SetObjAsHeadTarget` call unlikely | 46.320040% to 46.491737%; size 14,186 to 14,197 bytes, `0x570` frame | The call path is conditional on `action_target_ref`. Marking only that guard unlikely improves the larger block ordering; marking it likely scored 45.972343%. A nearby likely opponent-exclusion bit hint scored 46.329020% alone but 44.680676% when combined, so it was not retained. |
| Put `FreePlay` before `active_neutral_count` among the cover preheader's empty-asm address inputs | 46.491737% to 46.495330%; size stays 14,197 bytes, `0x570` frame | All 24 operand permutations were measured. Several orders tie at 46.495330%, while beginning with `FreePlay` scores only 46.252872% to 46.269040%. The retained order keeps `VADER_ADATA` and `party_under_cover` first, then passes `FreePlay` before the active count; input-operand order affects register allocation despite emitting no instructions. |
| Keep the interactive-array pointer and interaction-mode value live at the avoidance-mode branch | 46.447918% to 46.468033%; size 14,197 to 14,205 bytes, `0x570` frame | A zero-instruction `"r"` input for the array pointer alone scores 46.453663%; adding the mode value improves it further. The mode address scores 46.460846% with the pointer, while the mode value alone scores 46.455100%. Pinning the pointer to `edi`, `esi`, `eax`, or `edx`, or making it a read/write register operand, all regress. Operand values and placement change register lifetimes beyond the local branch. |
| Hand the initialized baddy cursor through an empty read/write register operand before the main object loop | 46.468033% to 46.504310%; moving `object = Obj` before the two cursor declarations then gives 46.516884%; size stays 14,205 bytes, `0x570` frame | The `+r` handoff immediately after cursor initialization changes the main-loop preheader GOT-load order and several stack-slot assignments, without emitted asm instructions. An input-only operand there scores 46.468390%; moving `+r` after `object = Obj` scores 46.450073%, and pinning the cursor to `esi` or `edi` scores 43.264366% or 39.469110%. Moving the `Obj` assignment alone before either cursor declaration produces the same 46.516884% score. After removing the later `MiniCutCam` memory operand, the handoff became score-neutral and was removed; the earlier `Obj` assignment remains useful. |
| Put the combat source block before the non-player wall and route blocks, with labels preserving wall → route → combat execution | 46.516884% to 46.520473%; size stays 14,205 bytes, `0x570` frame | The earlier layout experiment gained only 0.003589 points and was deferred. Combined with the baddy-cursor handoff and `Obj` assignment order, it gives the same small gain and is now retained for the match. It remains a 0.003595-point gain after the later `MiniCutCam` and baddy-handoff removals (46.529810% without the reordering versus 46.533405% with it). The executable path remains the same; source order by itself does not force GCC's call order. |
| Remove the empty input-memory operand on the cached `MiniCutCam` value after the baddy and block-layout changes | 46.520473% to 46.533405%; size 14,205 to 14,189 bytes, `0x570` frame | The earlier addressable cache had improved the 40.700430% layout, but on this layout it forces a stack home that now worsens the whole-function match. Keeping the plain cached local still beats reading `MiniCutCam` directly (46.495690%). Input or read/write register operands and moving the initializer into the `for` header compile identically to the plain local; a read/write memory operand scores 46.382540%. Guarding the load on a positive object count scores 44.323635%. Retest old compiler constraints after substantial data-flow changes. |
| Cache the first object-loop `HIGHGAMEOBJECT` bound in a local without an extra guard | 46.533405% to 46.537357%; size stays 14,189 bytes, `0x570` frame | The target loads the positive bound once and compares the loop counter to its cached value in `edi`. The current compiler still spills the cached local and compares against `[esp+0xe8]`, but the source change reassigns scratch stack slots and slightly improves the full diff. Declaring the bound before or after `mini_cut_cam` compiles identically. Wrapping the loop in a positive-bound guard and delaying the camera load scores 44.323635%. Forcing the bound through `edi` before or inside the loop scores 45.048492%–45.725933%; pairing it with a memory operand on `mini_cut_cam` still scores only 45.496407%–45.605602%. The compiler puts the bound in `edi` briefly, then spills it and reuses `edi` for the camera value. Earlier cached-bound probes on different layouts had regressed; their ranking did not carry over. |
| Pass the first-loop `high_flags` byte through an input-only `ecx` operand instead of an input-memory operand | 46.537357% to 46.554960%; size 14,189 to 14,173 bytes, `0x570` frame | A free register input scores 46.553520%; pinning to `ecx` adds a small gain and reflects the target's `movzx ecx, byte ptr [object+0x1f9]`. Pinning to `eax` or `edx`, making the operand read/write, typing the local as `i32`, or moving the marker after the eligibility check all regress. The prior addressable byte had helped an earlier layout, but here its stack home moves other locals unfavorably. Keep the marker immediately after the byte load. |
| Pass the cached first-loop object limit through an input-only register operand | 46.554960% to 46.572918%; size stays 14,173 bytes, `0x570` frame | The unconstrained input makes GCC test the loaded limit in a register before entering the loop, while it still saves a stack copy for the latch. A fixed `ecx` input ties the score but uses a different register; `eax`, `edi`, read/write register, input-memory, and read/write memory operands all regress. The target keeps the limit in `edi` through its latch, so the current gain is in the wider allocation, not complete recovery of that live range. |
| Pass the cached `MiniCutCam` value through an input-only `ecx` operand immediately after its load | 46.572918% to 46.595905%; size stays 14,173 bytes, `0x570` frame | The target loads this value after the positive bound test and stores it on the stack. The current compiler still loads it before the test, but the fixed `ecx` input changes the function-wide register allocation favorably. An `edx` input ties the score, though its preheader differs; `esi`, `eax`, and `edi` inputs score 44.715520%, 44.522270%, and 45.570404%. Moving the input into the loop or just before its use scores 46.289154%–46.454384%. The earlier input-memory operand on this value remains counterproductive. |
| Keep a volatile first-loop camera cache and request the `HIGHGAMEOBJECT` GOT address in `edx` | 46.595905% to 46.608480%; size 14,173 to 14,189 bytes, `0x570` frame | The volatile camera cache alone scores 46.579742%, while the fixed `edx` address alone scores 46.507904%. Together they leave the loop bound in `edi`, as in the target, and produce the best measured whole-symbol result. Forcing both `Obj` and the bound GOT addresses to their target registers lowers this combined layout to 45.860634%; local prologue register matches are not additive. |
| Remove the cover-global input-memory marker after the new first-loop allocation | 46.608480% to 46.612070%; size stays 14,189 bytes, `0x570` frame | The `party_under_cover` read marker improved an older 45.764366% layout, but now shifts scratch allocation slightly against the target. Ablating the other 14 standalone markers on the 46.608480% layout did not improve it; two first-loop input markers tied its score, while all other removals lowered it. Retest older hints after register-allocation changes. |
| Reorder the four cover-scan GOT-address inputs after removing the cover-global memory input | 46.612070% to 46.613865%; size stays 14,189 bytes, `0x570` frame | All 24 orders were remeasured. `VADER_ADATA`, `active_neutral_count`, `FreePlay`, then `party_under_cover` wins on this layout. The former order won on the earlier layout with the memory input; operand order changes GCC's live ranges even though the marker emits no bytes. |
| Pin the first-loop `Obj` GOT address to `ecx` alongside the bound address in `edx` | 46.613865% to 46.617817%; size stays 14,189 bytes, `0x570` frame | This emits the `Obj` GOT load into `ecx` rather than the target's `edi`, but improves later alignment. Pinning it to `eax`, `esi`, or `edi` scores 46.547413%, 44.658764%, or 45.866020%. Reversing the two address operands scores 46.608480%. The winning register is not necessarily the locally matching one. |
| Stage both bytes of the `field_0xef9` update in addressable locals | 38.002514% to 38.264730% with the shifted byte, then to 38.315372% with the cleared/combined byte; size 14,117 to 14,133 bytes | The target writes the shifted bit and the cleared byte into separate stack slots before combining them. Zero-instruction `+m` constraints on those two `u8` locals recover the `add`/`and`/stack-store/reload/`or` sequence. This is a measured stack-allocation match for this expression, not a general reason to force all bit math into memory. |
| Hint the following `field_0xef9` sign-bit path as likely after staging its bytes | 38.315372% to 38.515804%; size 14,133 to 14,125 bytes | The target uses `jns` to skip a fallthrough copy of three fields. The hint now gives the same local branch and copy layout. The same hint had lowered an older layout, so retest likelihood hints after data-flow changes. |
| Hint the collision-priority `ai.field_0x1e7 & 0x80` branch as likely on the later layout | 38.515804% to 39.016884%; size 14,125 to 14,117 bytes | The target tests the sign bit and uses `jns` to skip the fallthrough `0x4000` priority update. An earlier trial of the same hint lowered a 37.907690% build. After the `field_0xef9` changes, the hint improves the whole function, showing that branch-layout results depend on distant register and block changes. |
| Reduce the stack-frame placeholder from three 16-byte arrays to one after adding the two `field_0xef9` stack bytes | 39.016884% to 39.020473%; size stays 14,117 bytes | The new addressable bytes had grown the current frame from the target's `0x570` to `0x590`, shifting the four pointer-array bases up by `0x20`. Removing two obsolete padding arrays restores the `0x570` frame and target array bases at `esp+0x170/0x270/0x370/0x470`. Recheck frame and local offsets after every added addressable temporary; an old padding calibration can become stale. |
| Hoist both `field_0xef9` scratch bytes to the main-loop scope, then use a 32-byte frame placeholder | 39.020473% to 39.020832%; size stays 14,117 bytes | Hoisting moved the two byte slots next to each other and shrank the frame to `0x560` with the prior 16-byte placeholder. Enlarging that placeholder to 32 bytes restored the target's `0x570` frame and list-array bases. Hoisting only either byte produced the same 39.020832% score. The byte slots still differ from the target's `esp+0x100/0x11c`, so this is a one-instruction gain, not a complete local layout match. |
| Make the first object-loop index available in `edx` through a zero-instruction input constraint | 39.020832% to 39.445040%; size 14,117 to 14,061 bytes | The target initializes and carries this loop counter in `edx`; the prior build used `ecx`. An empty volatile asm with a `"d"(index)` input at the loop body's start makes GCC initialize `edx` and improves the full alignment. An empty asm without an operand compiled identically and did not fix the register. The loop still initially jumps to its test, so the counter register is only one part of that mismatch. |
| Cache first-loop `flags_high` in an addressable byte local alongside the index constraint | 39.445040% to 39.629670%; size 14,061 to 14,029 bytes | The target loads the byte at `object+0x1f9` and saves it for the later `MiniCutCam` test. An input-only `m` constraint on a cached byte forces a stack home without requiring a reload before the eligibility test. On the alternative tail-index layout, this same cache lowered the score; combine local layout choices before judging them. This local grows the current frame from `0x570` to `0x580`, moving pointer-array bases from target offsets `0x170/0x270/0x370/0x470` to `0x180/0x280/0x380/0x480`; the score rose despite those new offset mismatches. |
| Recalibrate the placeholder after adding the cached `flags_high` byte | 39.629670% to 39.632904%; size stays 14,029 bytes | Reducing the 32-byte placeholder to 16 bytes restores the target's `0x570` frame and four pointer-array bases. The whole-score gain is small because the surrounding register flow is still different, but stack offsets are again aligned. |
| Require the `Obj` and `HIGHGAMEOBJECT` GOT addresses in that order before the first loop | 39.632904% to 39.634340%; size stays 14,029 bytes | A zero-instruction input-only asm makes GCC emit the two GOT loads in the target's order. The subsequent dereferences and loop register choices remain different, so this recovers only a few aligned instructions. |
| Require the loaded `Obj` pointer before the first-loop bound read | 39.634340% to 39.636135%; size stays 14,029 bytes | A second input-only asm makes the first four preheader operations follow the target's GOT/GOT/dereference/dereference order. The GOT destination registers and later loop layout remain different. |
| Hint the first-loop nonzero action-frame count as likely | 39.636135% to 40.008263%; size 14,029 to 14,035 bytes | The target's loop keeps the decrement branch near the main path, whereas the unhinted build favors the timer/reset path. This narrowly placed hint improves whole-function block alignment even though it does not make the loop's opening fully match. |
| Constrain the updated first-loop index in `edx` at the loop latch, then retain the `edx` input at body entry | 40.008263% to 40.108837% with only the latch; to 40.320763% with both, size stays 14,035 bytes | Pinning the increment result at the latch changes the carried counter's lifetime differently from changing the body operand to read/write. The latter had regressed on both earlier layouts. The pair improves whole-symbol alignment but does not yet recover the target's body-first loop entry or its `esi` action-byte temporaries. |
| Hoist the addressable first-loop `flags_high` byte to function scope and restore the frame size | 40.320763% to 40.678880% before frame recalibration; 40.682114% after, size 14,035 to 14,091 bytes | Declaring the cached byte outside the loop changes its lifetime enough that GCC also schedules the `TouchControlsActive` GOT load before the body, closer to the target. It initially shrinks the frame to `0x560`; enlarging the stack placeholder from 16 to 32 bytes restores `0x570` and the four pointer-array bases. The change is larger than the direct high-byte load and illustrates that local lifetime affects hoisted globals and stack allocation together. |
| Place a field-scoped read/write compiler barrier on the main-loop AI-update byte after the awareness merge | 40.682114% to 40.700430%; size stays 14,091 bytes | The target reloads this field at the later AI-update decision, while the source carries an earlier decision. The zero-instruction barrier slightly improves whole alignment but by itself does not force a re-read of the carried `process_ai` value. A later direct-field expression with a separate local did force different data flow, but scored worse; see below. |
| Cache `MiniCutCam` in an addressable integer before the first loop | 40.700430% to 40.904095%; size stays 14,091 bytes | The input-only memory operand gives the cached camera value a stack home at `esp+0xec` and improves register scheduling around the first-loop preheader. GCC loads it before the positive `HIGHGAMEOBJECT` test, while the target loads it after; making that source load conditional lowered the score, so the higher-scoring form is retained. |
| Initialize `follow_offset` before the first player scan | 26.834412% to 26.854885% | Its former declaration just before the second player scan scheduled a few zero-init and pointer instructions differently. Moving the declaration before the first scan changed only 19 aligned instruction slots; it did not remove the wider `xmm1` spill gap. |
| Update the player-scan cover accumulator with `&=` on the model flag test | 26.854885% to 26.952227% at that stage | This preserves the boolean result but emits a shifted bit and `and` instead of a conditional assignment. At that stage the current build did not reproduce the target's seven `cmovne` decisions. Using `u8` for the accumulator lowered this form to 26.327227%, and `&&` lowered it to 23.539510%. Recheck micro choices after larger layout changes; the conditional assignment later became better. |
| Place the four pointer arrays in one local struct, ordered neutral, interactive, goodies, baddies | 26.952227% to 28.821121%; size 13,485 to 13,728 bytes | GCC allocated four separate local arrays in a different role order from the target, regardless of their declaration order. The struct fixes their relative offsets: the target's array bases are at `esp+0x170/0x270/0x370/0x470` with a `0x570`-byte frame; the new build uses `esp+0x140/0x240/0x340/0x440` with a `0x540`-byte frame. The arrays therefore have the same frame-relative addresses. The remaining `0x30` frame-size difference is in other locals. |
| Advance a `GameObject_s *` through the main AI object loop, rather than recomputing `&Obj[index]` | 28.821121% to 31.338720%; size 13,728 to 13,672 bytes | The original decompilation advances its object pointer by `0x10e4` on each iteration; the earlier current build multiplied the index by `0x10e4` and reloaded `Obj`. This is a significant control-flow and register-lifetime difference across the largest loop. The loop index had no other uses and the pointer form preserves behavior. |
| Represent the four pointer buffers as one `APIOBJECT *object_lists[4][64]` | 31.338720% to 31.901580%; size 13,672 to 13,991 bytes | The two-dimensional array keeps the desired frame-relative buffer positions (`esp+0x130/0x230/0x330/0x430` in a `0x530`-byte frame) and, unlike the struct-of-arrays trial, lets GCC vectorize the neutral-to-baddie append into a four-pointer loop with `movdqa` and `movdqu`, as in the original. The compiler's alias analysis distinguishes these two equal-layout source forms. |
| After the pointer-loop and two-dimensional-array fixes, update `all_under_cover` with `if (model flag missing) all_under_cover = 0` | 31.901580% to 32.869250%; size 13,991 to 14,047 bytes | This earlier source form had lost to bitwise `&=`, but the larger stack and register changes reversed their ranking. The current build still emits seven `cmove` rather than the target's seven `cmovne`; the gain comes from the overall instruction alignment. Retest local codegen choices after structural changes. |
| In `TestWalkAround`, add the rotated displacement before the other object's position for X and Z | Caller remains 32.869250%; helper reaches 99.988370% at 365 bytes | The original helper's last X and Z updates load `difference` first and add the other position. The former source reversed those float operands. After reordering, a GOT-aware helper comparison (with the current clone symbol renamed in a temporary binary) shows only one differing `mulss` constant address; both addresses hold the same `10430.3779296875f` bits, and all other 85 aligned instructions match. Floating addition order can affect signed zero and NaN propagation, so keep the observed order. |
| Cache `player` once before the second eight-player scan | 32.869250% to 33.029810%; size remains 14,047 bytes | The target decompilation loads the active player before the scan and compares it to `Player[1]` during the swap check. A local `active_player` changes the current compiler's register and load scheduling favorably. This is distinct from caching the `Player` array's GOT pointer, which had regressed in an earlier trial. |
| Give the wall-ray hit and miss branches separate second `GameRayCast` calls | 33.029810% to 34.580460%; size 14,047 to 14,067 bytes | The target has three ray-cast call sites: the first cast and a second cast on each first-result branch. Sharing a single second call removed that layout and left one of the sparsest instruction intervals. Duplicating the second call in source restores three emitted sites and improves whole-function alignment. |
| Advance an `APIOBJECT **goody_cursor` alongside the neutral cursor on the current layout | 34.580460% to 34.823635%; size 14,067 to 14,099 bytes | The target's collection paths advance pointers after storing goodies. This cursor helps after the array and ray-control changes, even though the same combination regressed at an earlier baseline. |
| Advance an `APIOBJECT **interactive_cursor` for both interactive-object collection paths | 34.823635% to 35.119614%; size 14,099 to 14,147 bytes | The target decompilation has a cursor initialized to the interactive array and increments it after both stores. The cursor improves matching when combined with the retained neutral and goody cursors. |
| Initialize the interactive cursor just before the main object loop | 35.119614% to 35.200790%; size stays 14,147 bytes | The target initializes this cursor after the second player scan. Moving the source initialization closer to its first use changes register lifetime and improves alignment slightly. |
| Resolve the wall-ray direction within the first-cast hit and miss branches | 35.200790% to 35.233837%; size 14,147 to 14,139 bytes | The target uses branch-local distance decisions after its three ray calls. The source now handles first-hit/second-hit, first-hit/second-miss, and first-miss/second-hit cases explicitly while preserving the previous float comparisons, including unordered behavior. This modestly improves alignment after the larger three-call fix. |
| Cache the idle object's `pad_gamepad` pointer before clearing its fields | 35.233837% to 37.488148%; size 14,139 to 14,059 bytes | The target loads the pad once and clears the adjacent fields with two `movdqu` stores. Consecutive field stores through a local pointer let GCC use the same wide-store form. Declare the pointer before the main-loop `goto interaction`, then assign it at the clear site; C++ rejects a jump across its initialization. The whole-function score improved substantially on the current layout, despite this experiment having produced an unusable 0.0% alignment on an older layout. |
| Invert the party-cover accumulator to `any_uncovered`, initialized to zero and set to one when an eligible player's model lacks the cover flag | 37.488148% to 37.549570%; size 14,059 to 14,051 bytes | This preserves the final cover decision and changes GCC's register selection slightly. The target's seven `cmovne` selections are still missing, but the whole-function alignment improves. Replacing the conditional set with `any_uncovered |= flag_missing` lowered the score to 36.753230%, despite making the function eight bytes smaller. |
| Express the inverted cover update as a ternary choice between `1` and the previous accumulator | 37.549570% to 37.692528%; size stays 14,051 bytes | The two equivalent test polarities compile identically on this layout. The ternary improves whole-function alignment, although the current compiler still emits eight `cmove` and no `cmovne` for the scan. |
| Mark the complete first-loop eligibility condition as likely with `__builtin_expect` | 37.692528% to 37.798850%; size stays 14,051 bytes | Hints on the full conjunction affect block scheduling differently from the earlier hint on only the first skip test, which regressed. This does not yet make the first-loop body fall through as in the target, so treat it as a small local alignment gain. |

## Investigated dead ends and remaining differences

- A `baddy_cursor` replacing all three indexed baddie stores lowered the
  current 37.488148% baseline to 37.190730% and grew the function from
  14,059 to 14,107 bytes. The previous pointer-cursor trials also regressed
  on earlier layouts; the indexed stores remain in the source. Pointer
  cursors help the neutral, interactive, and goody lists here, but baddie
  collection has a different live-range cost.
- Retesting the three-store baddie cursor on the later 38.515804% layout
  measured 38.169540% (14,173 bytes). Using it only in the second player
  scan measured 38.330820% (14,157 bytes), and only in the main object
  loop, initialized from `baddies + baddy_count`, measured 38.310703%
  (14,157 bytes). All three forms remain below the indexed-store baseline;
  the cursors were removed.
- Marking only the first first-loop skip test as unlikely with
  `__builtin_expect` lowered the 37.488148% baseline to 33.598060% and grew
  the function to 14,067 bytes. The loop body still needs a structural fix;
  branch likelihood on that one condition does not recover its placement.
- Marking the first-loop touch-control capability branch likely lowered the
  later 37.798850% baseline to 37.370330% and grew the function from 14,051
  to 14,099 bytes. It did not recover the target's `jns`/`js` polarity. The
  hint was removed; source-level branch probability is not the sole cause of
  the inverted sign branches.
- Naming `HIGHGAMEOBJECT` in a cached `const i32` before the first loop,
  while retaining the `<` test and full-condition likelihood hint, compiled
  identically at 37.798850% and 14,051 bytes. GCC already caches that bound;
  the local did not correct the order of the `Obj` and `HIGHGAMEOBJECT` GOT
  loads. The redundant local was removed.
- Replacing the neutral-to-baddie copy's pointer aliases with direct
  `object_lists[3][...] = object_lists[0][...]` indexing removed the visible
  overlap guard from its vector loop, but the whole function shrank from
  14,051 to 13,696 bytes and the linked-GOT match fell from 37.798850% to
  24.247126%. Both forms still emitted one `movdqa` and three `movdqu`
  instructions in the caller. The direct form was reverted: a local alias
  improvement can disrupt global stack and register layout badly.
- On that same layout, enclosing the whole first-loop body in a positive
  likely-eligibility `if` instead of using `continue` also compiled
  identically at 37.798850% and 14,051 bytes. GCC still moved the body away
  from the target's fallthrough position. The simpler loop form was restored.
- A zero-instruction `+r` constraint immediately after loading `Obj` forced
  its GOT load ahead of `HIGHGAMEOBJECT`, closer to the target prologue, but
  the whole match fell from 37.798850% to 37.768320% at the same 14,051-byte
  size. The target then moves the bound into `edi`; the probe still kept it
  in `edx`. Load order alone did not fix register allocation, and the probe
  was removed.
- Binding the cached first-loop bound to `edi` through a local-register asm
  constraint produced the target's `test edi, edi`, but lowered the whole
  match from 37.798850% to 37.391163% at the same 14,051-byte size. It also
  put the `Obj` GOT address in `edx` instead of the target's `edi`. Forcing
  one target register shifts other live values; the probe was removed.
- Forcing the main-loop object pointer into `edi` at loop entry recovered
  that target register in the collision-priority region, but changed store
  ordering and other live registers. The 37.907690% baseline fell to
  36.567528%, with size 13,977 bytes. The zero-instruction constraint was
  removed. Register identity must arise from matching lifetimes, not from
  pinning one value at loop entry.
- A zero-instruction `esi` clobber inside the main loop moved the object
  pointer to `edx` and lowered 39.016884% to 33.535920% (14,125 bytes).
  Clobbering both `esi` and `edx` inside the loop put the object in the
  target's `edi`, but lowered the whole match to 36.806393% and shrank the
  function to 13,888 bytes. Placing that dual clobber once before loading
  `Obj` scored 35.840520% (14,133 bytes); placing it after the load scored
  36.075073% (14,077 bytes). A matching object register alone is not a
  sufficient match for the loop's control flow and other live values; all
  clobbers were removed.
- Putting the two `field_0xef9` bytes in one forced-memory struct with a
  27-byte gap reproduced their target relative spacing (`0x1c`), but GCC
  placed the struct at `esp+0x143/0x15f`, not the target's
  `esp+0x100/0x11c`, and the frame/list bases moved down `0x10`.
  The whole score was 39.016884% (14,117 bytes), below the hoisted-byte
  39.020832% layout. Declaring the struct before instead of after the
  list arrays compiled identically. Relative field offsets alone do not
  control an aggregate's absolute stack placement; the struct was removed.
- Forcing the main-loop object pointer through a `+m` stack slot at loop
  entry made GCC initialize the interactive cursor in `edi`, but the loop
  then kept the object in `edx`; the 38.515804% baseline fell to
  35.304596% (14,076 bytes). Forcing only `interactive_cursor` through a
  `+m` slot fell to 37.520115% (14,109 bytes). Neither isolated stack
  constraint reproduces the target's cursor-to-object register handoff;
  both were removed.
- Swapping only the source order of `object = Obj` and the interactive
  cursor initializer compiled identically at 38.515804% and 14,125 bytes.
  GCC's register handoff is unaffected by those independent declarations.
- A temporary `optimize("no-reorder-blocks")` attribute produced a 13,786-byte
  function and a 0.0% whole-symbol alignment from the GOT-aware diff. Its
  first prologue still contains many identical instructions, so the zero
  score reflects a failed global alignment after broad layout changes. The
  target's block layout cannot be recovered by disabling that pass for the
  whole function; the attribute was removed.
- Guarding the first object loop with a positive cached bound and ending it
  with `index != bound`, as the target assembly appears to do, lowered the
  current 37.488148% baseline to 34.934986% (14,067 bytes). A similar loop
  form had also regressed on an earlier layout. The decompiled loop test does
  not by itself determine the original source control flow.
- A zero-instruction compiler barrier after the first collision
  `TestWalkAround` call lowered the 37.488148% baseline to 36.951508% and
  grew the function to 14,076 bytes. Forcing a separate call path this way
  does not reproduce the target's surrounding register flow; the barrier was
  removed. The target still has three helper call sites while the compiler
  currently merges two source calls.
- Repeating the call-tail experiment at 37.798850% confirms that a
  branch-local zero-instruction memory barrier after either of the first two
  source calls gives three emitted calls, but scores only 37.527298% or
  37.509340% (14,068 bytes). A literal `nop` after either call also gives
  three sites but scores 37.357400% or 37.006107%. The call count alone is
  insufficient; GCC must also recover the target's argument setup and block
  order. All four markers were removed.
- A later 39.629670% call census confirms 69 target calls and 68 current
  calls, with the target's third `TestWalkAround` site still merged away.
  An empty volatile asm with no memory clobber after the first source
  call restored three sites but scored 38.965157% (14,058 bytes); placing
  it after the second scored 38.903020% (14,058 bytes). Placing it after
  the third kept two sites and scored 39.582973% (14,029 bytes). The
  markers were removed. Even a zero-instruction side-effect marker
  changes this loop's global block alignment more than the extra call
  helps.
- An input-only memory constraint on the first path's resolved collision
  priority after its helper call also restored the third call site, but
  scored 38.727730% (14,065 bytes) against the 39.629670% baseline.
  Forcing a branch-specific memory dependency does not yet reproduce the
  target's call argument and follow-on timer layout; it was removed.
- A temporary `optimize("no-crossjumping")` attribute on `GameAIProcess`
  restored all three `TestWalkAround` call sites, proving RTL crossjumping
  accounts for the merged current call. It also grew the function from
  14,051 to 14,629 bytes and dropped the match from 37.692528% to
  27.355963%. The attribute was removed. The target likely avoids merging
  these paths because their surrounding instructions differ, not because
  crossjumping is globally disabled.
- A zero-instruction `+r` constraint on the second collision pointer just
  before the third source `TestWalkAround` call kept only two emitted call
  sites and lowered 37.692528% to 37.554596% at the same 14,051-byte size.
  Merely forcing that pointer into a register does not distinguish the tails
  enough to match the target. The constraint was removed.
- Marking only `TestWalkAround` as `noinline` produced an identical
  37.692528%, 14,051-byte caller with two emitted clone calls. That attribute
  does not address the RTL tail merge; it was removed.
- Caching the `Player` array address in a plain first-scan local compiled to
  the identical 37.692528%, 14,051-byte function. GCC folded the local away;
  merely naming the pointer does not reproduce the target's long-lived GOT
  register. The redundant local was removed.
- On the later 37.692528% layout, a `bool all_under_cover` initialized true
  and updated with the target decompiler's `model_flag ? previous : false`
  form measured 37.365303% and 14,059 bytes, still with eight `cmove` and no
  `cmovne`. The current inverted integer accumulator remains preferable by
  the whole-function metric. Decompiled local types do not guarantee the
  original source type or compiler register choice.
- Holding the party-cover result in a local and assigning
  `party_under_cover` once after the first player scan, as Ghidra suggests,
  lowered the later 37.692528% layout to 37.405533% and grew the function
  to 14,059 bytes. The direct global writes were restored. The decompiler's
  final store does not yet translate to a better source shape for this build.
- Doing the same local carry only for the second player scan, where the
  target decompilation reads `party_under_cover` before the loop and stores
  it afterward, lowered 37.692528% to 37.571480% at the same 14,051-byte
  size. The original global update was restored. This source-level lifetime
  is insufficient to reproduce the target's register allocation.
- Naming and reusing both collision priorities as local `u16` values lowered
  37.692528% to 37.518320% and grew the function from 14,051 to 14,068
  bytes. The current build still emitted only two `TestWalkAround` calls.
  The direct member accesses were restored; explicit source caches do not
  create the target's distinct collision tails.
- Merely swapping the separate `neutral_objects` and `baddies` declarations
  generated the same 26.952227% score and 13,485-byte function. GCC chose
  stack slots independently of their source declaration order. The explicit
  struct layout above was needed to control relative array placement.
- The aligned diff still shows a large first-object-loop block-order gap: the
  target falls into the eligible-object body near `17d270`, whereas the
  current build branches into it near `2bf681`. Adding
  `__builtin_expect(..., 0)` to mark the skip condition unlikely lowered the
  struct-layout baseline from 28.821121% to 27.848060% (size 13,728 to
  13,720). The hint was removed; branch likelihood alone does not recover the
  target loop layout.
- After grouping the arrays in a struct, the neutral-to-baddie append loop
  became scalar in the current build, while the target copies four pointers
  per loop iteration. Marking the disjoint source and destination pointers
  `__restrict__` compiled identically at 28.821121%. Manually copying four
  entries per iteration, with a scalar tail, lowered the whole-symbol score
  to 28.461926% (13,745 bytes). Both trials were reverted. The target's
  unroll depends on broader loop and alias analysis, not the local spelling
  alone. A two-dimensional array subsequently recovered the SIMD copy and is
  the retained form.
- The retained two-dimensional array version still emits a runtime overlap
  guard before its SIMD copy; the target has no such guard. `__restrict__` on
  the two base pointers compiled identically at 31.901580%. Creating fresh
  restricted source/destination pointers immediately before the copy lowered
  the score to 31.779095% (14,039 bytes). Neither form removed the guard, so
  the plain indexed copy was retained.
- The fresh 37.798850% decompile confirms the target starts its four-pointer
  neutral-to-baddie copy at four elements; the current build first checks
  possible source/destination overlap and starts the vector loop at ten.
  `#pragma GCC ivdep` and fresh local `__restrict` aliases both compiled
  identically at 37.798850% (14,051 bytes). Telling GCC that the sum of the
  two counts cannot exceed the 64-slot destination lowered the score to
  37.271190% (14,083 bytes). These were reverted. The guard depends on the
  wider array layout and alias analysis, not just a loop-local hint.
- At the same 37.798850% baseline, nesting the collision-priority comparisons
  to mimic Ghidra's target branches lowered the score to 37.497486% and size
  to 14,017 bytes. The flat priority update/equality/update source remains
  preferable; a decompiler's nested branches are not a reliable source
  spelling for GCC.
- Flattening the same four buffers to `APIOBJECT *object_lists[256]` with
  pointer offsets of 0, 64, 128, and 192 compiled to the same 31.901580%
  and 13,991-byte symbol as the two-dimensional array. The more readable
  two-dimensional form was restored.
- Caching `Player[1]` alongside the retained `active_player` local before the
  second player scan lowered 33.029810% to 31.723420% (14,047 to 14,039
  bytes). The target loads `Player[1]` inside the swap condition, so only the
  active-player value remains cached.
- Replacing `Player[1 - index]` with explicit `if (index != 0)` loads of
  `Player[0]` and `Player[1]`, on the 33.029810% baseline, collapsed the
  whole-symbol score to 3.503951% despite the same 14,047-byte size. The
  decompiler's branch presentation is not a safe source template for this
  scan; the indexed form was restored.
- Reversing the duplicated wall-ray source branches to put the first-ray miss
  path first lowered 34.580460% to 33.511852% (14,067 to 14,083 bytes).
  Retain the first-ray hit path as the source-level `if` arm even though the
  target decompiler presents its miss path first.
- Duplicating the final wall-direction choice inside both ray-result branches
  lowered 34.580460% to 34.015446% (14,067 to 14,083 bytes). Separate ray
  calls improve the match, while the shared final direction decision better
  matches the current target alignment.
- Adding a baddie cursor alongside the retained neutral and goody cursors
  lowered 34.823635% to 34.740660% (14,099 to 14,147 bytes). Using only
  neutral and baddie cursors lowered it to 33.904810% (14,115 bytes).
  A baddie cursor only in the second player scan measured 34.744250%
  (14,131 bytes), and one only in the main object loop measured 34.784843%
  (14,131 bytes). Retain neutral and goody cursors with indexed baddie writes
  at this stage.
- On the neutral, goody, and interactive cursor layout, adding a baddie
  cursor lowered 35.119614% to 34.816810% (14,147 to 14,195 bytes).
  Keeping only neutral and interactive cursors measured 34.631107%
  (14,115 bytes). Retain the neutral, goody, and interactive cursors.
- On the 32.869250% layout, spelling the cover update as
  `mask ? all_under_cover : 0` compiled to the same score and 14,047-byte
  size as the direct conditional assignment. The simpler `if` form remains.
  Initializing the accumulator to zero and restoring an explicit saved value
  when the mask is set also compiled identically, including seven `cmove`
  instructions rather than the target's `cmovne`; GCC simplifies these
  source variations to the same SSA selection.
- At the earlier 25.989584% baseline, the original `TestWalkAround` helper
  was an `.isra` clone of 365 bytes with a squared-distance guard inside it,
  while the current `.isra...part` clone was 340 bytes and GCC had copied the
  guard into the caller. The original caller had three call sites and nine
  `mulss` instructions; the earlier current caller had two call sites and
  eighteen `mulss` instructions. A temporary `noinline` experiment removed
  the extra multiplies and reached 26.986351%, but generated a 404-byte
  helper with a different prologue and still only two call sites. It was
  reverted. After the later array and loop changes, the current helper is a
  365-byte `.isra.19` clone with the guard inside it; the caller still has two
  call sites. Do not add compiler attributes just to force assembly shape; see
  [source conventions](05-source-conventions.md).
- All three source paths into `TestWalkAround` still exist. In the earlier
  26.854885% assembly, the opposite-orientation path at `2c0ca8` prepared
  its arguments then jumped to the call at `2c06b8`; the other call was at
  `2c05df`. The original has distinct calls at `18082b`, `18098c`, and
  `180a0e`. The latest caller still has two call sites, despite the helper
  having recovered its 365-byte shape. Thus the call-count gap includes tail
  merging, not a lost source branch. Moving the subsequent `first_ai` and
  `second_ai` pointer loads before the decision kept two call instructions
  and lowered 26.854885% to 26.436422% (size 13,549 to 13,581); that
  source trial was reverted.
- An isolated `-fno-partial-inlining` override for `gameobjects.cpp` removed
  the copied guard multiplies from the caller (`18` to `9` `mulss`), but the
  linked-GOT match fell from 26.854885% to 26.743895%, the caller shrank from
  13,549 to 13,453 bytes, and the helper still had only two call sites.
  Partial inlining is one contributor to the mismatch, not its sole cause.
  The normal build flags were restored.
- Replacing the helper's guarded body with a semantically equivalent early
  return generated identical machine code and left the score at 26.834412%.
  Flattening the three collision-call branches lowered it to 26.654095%.
  Neither source spelling prevented GCC's partial helper split.
- Moving the unchanged static `TestWalkAround` definition from immediately
  before `GameAIProcess` to near the top of `gameobjects.cpp` did not change
  the emitted helper or caller: the same `.isra.19.part.20` clone remained at
  `0x2b7850`, the caller remained at `0x2bf4f0`, and the score stayed
  26.952227%. Definition order alone does not explain the target's earlier
  `.isra.10` clone or the caller mismatch. The original source order was
  restored.
- The original final `oneAtOnce_MaintainArray` code has two call sites, while
  the current code merges them into one. Explicitly duplicating the source
  call emitted two sites but lowered the whole-symbol score from 25.989584%
  to 17.508621%. Call count alone does not identify the correct source shape.
- An earlier idle-gamepad clear experiment produced two target-like `movdqu`
  stores locally, but the whole-symbol score became 0.0% in that experiment.
  Inspection of the fork's JSON showed 251 identical instructions in its
  alignment, so 0.0% did not mean there were no local matches. `memset` did
  not emit those stores. Repeating the cached-pointer experiment after the
  larger loop and array changes raised the score to 37.488148%, and the
  pointer form is now retained. Revisit local instruction wins after major
  source-layout changes rather than permanently discarding them.
- Rewriting the first object-loop skip as a positive enclosing `if` produced
  identical code at 26.854885%. Reshaping that loop as an explicit `do` loop
  with a hoisted `HIGHGAMEOBJECT` bound lowered the score to 25.421696% and
  changed the size from 13,549 to 13,552 bytes. The original assembly has a
  bottom-tested pointer walk, but forcing that source structure did not
  recover its register choice or branch layout. Capturing the old capability
  word in a named local before clearing bits also compiled identically; it
  did not remove the caller's stack spill of that word.
- The original player scan keeps the `Player` GOT pointer in `esi` and stores
  its cover decision on the stack; the current scan keeps the decision in
  `esi` and reloads `Player`. Making the decision `volatile` forced memory
  accesses but dropped the linked-GOT score from 26.854885% to 16.587284%
  (size 13,549 to 13,010). It changes much more than register allocation and
  was reverted.
- A later zero-instruction `+m` compiler constraint on `all_under_cover`
  forced its initialization to be addressable, but did not recover any target
  `cmovne` instruction: the current build still had nine `cmove` and zero
  `cmovne`. It lowered the 37.488148% baseline to 37.290947% at the same
  14,059-byte size and was removed. An addressable stack slot alone does not
  reproduce the target's conditional selection.
- With the later inverted `any_uncovered` accumulator, a `+m` constraint
  inside every eligible player update did produce eight `cmovne`
  instructions. Placing it after the update lowered the 38.515804% build
  to 34.824352% (14,297 bytes); placing it before lowered the score to
  33.860634% (14,193 bytes). Recovering one mnemonic pattern by forcing
  every iteration's accumulator through memory disrupted far more code.
  Neither constraint was retained.
- Special-casing `index == 0` inside the eight-player cover loop changed GCC's
  unrolling drastically: the function shrank from 14,051 to 11,891 bytes and
  the linked-GOT match fell from 37.692528% to 33.993176%. It produced only
  one `cmovne`, not the target's seven. The case was removed. Keep the loop
  uniform unless there is stronger evidence that the source singled out a
  player.
- Typing the later inverted `any_uncovered` accumulator as `bool` instead of
  `i32` lowered the 37.798850% layout to 37.132540% and shrank it from
  14,051 to 14,035 bytes. It still emitted eight `cmove` and no `cmovne`.
  The integer form was restored; a source boolean does not recover the
  target's conditional-move polarity.
- Caching the first-scan player's `model_flags` before its cover-context
  branch, then using the cache for both `0x204` and `0x80000` masks, lowered
  37.798850% to 35.053160% and grew the function from 14,051 to 14,059
  bytes. The early load changes scheduling across the branch; the direct
  field accesses were restored. This repeats an older regression on the
  newer layout.
- A 32-bit `all_under_cover` with the target-like `flag ? previous : 0`
  ternary measured 36.932830% and 14,051 bytes against the 37.798850%
  inverted-accumulator baseline. It still emitted eight `cmove` and zero
  `cmovne`. The source expression alone does not account for the target's
  polarity; the inverted form was restored.
- Replacing that ternary with an explicit zero-initialized `next_cover`,
  conditionally assigned the old cover value, compiled to the same
  36.932830%, 14,051-byte function with eight `cmove`. GCC canonicalizes
  the two source shapes; an explicit temporary does not recover the target
  selection. The inverted accumulator remains retained.
- In the second player scan, `Player[1 - index]` makes the current compiler
  negate the index before indexing the array. The target instead branches on
  whether the first of the two indices is being processed and directly loads
  `Player[1]` or `Player[0]`. Spelling that choice as `index == 0 ? Player[1] :
  Player[0]` removed the negation locally, but the fork's whole-symbol score
  became 0.0% with a different alignment. Using `Player[index ^ 1]` measured
  13.071480%. Both were reverted; do not infer an overall improvement from
  the local instruction change alone. An explicit branch that loaded
  `Player[1]` before the swap decision, mirroring the target's apparent
  order, also scored 0.0% and was reverted. Repeating the direct two-branch
  selection on the later 37.692528% layout measured 35.834410% and 14,059
  bytes, so the indexed form remains retained.
- The original function allocates `0x570` bytes of stack space; an earlier
  current build allocated `0x530`. Hoisting vector locals helped earlier, but eight
  declaration-order variations did not close this difference. Stack size
  alone does not prove which source local or lifetime is missing. The bundled
  GCC 4.7 compiler rejects `-fstack-reuse=none`, so that modern diagnostic
  flag cannot test the hypothesis in this toolchain.
- Reducing the one retained 16-byte placeholder to eight bytes moved the
  current frame and list arrays down by `0x10` (`0x560` frame and
  `esp+0x260/0x460` array bases) and returned the score to 39.016884%
  at 14,117 bytes. The 16-byte placeholder was calibrated for the
  block-local byte temporaries; after hoisting them, a 32-byte placeholder
  is retained.
- On the later cached-gamepad layout, the current frame was `0x540` while
  the target remained `0x570`. A temporary, zero-instruction 48-byte local
  forced the exact `0x570` allocation, but the match moved only from
  37.488148% to 37.491737% and the function size stayed 14,059 bytes. The
  probe was removed. Merely matching the total frame size does not recover
  the missing register lifetimes or stack-slot accesses.
- Repeating a 48-byte zero-instruction local after the later touch-branch
  change raised 37.907690% to 37.911278% at the same 14,083-byte size. It
  matches the target's `0x570` prologue but leaves the later `esp` offsets
  shifted only partway toward the target. Declaration immediately before
  versus after the four buffers compiled identically. A 56- or 64-byte local
  gave a `0x580` frame and 37.907690%, with array bases past the target;
  three separate 16-byte locals match both the `0x570` frame and the target
  array bases at `esp+0x170/0x370`, while scoring the same 37.911278%.
  They were retained as a layout placeholder at that stage; after the later
  `field_0xef9` stack-byte changes, only one is retained. A second zero-instruction
  read/write constraint after the main object loop, intended to keep all
  three locals live through it, compiled identically at 37.911278%.
  Wrapping the
  padding and 2D buffers in one struct instead produced a `0x560` frame,
  14,020 bytes, and 35.520115%, so the plain 2D buffers were restored.
- Naming the `Player` array pointer once before the second scan changed GCC's
  register lifetimes and lowered 37.911278% to 35.603810% (14,027 bytes).
  The original's `esi` use in that scan does not arise from a simple source
  pointer alias; the local was removed.
- Binding that second-scan `Player` pointer to `esi` with a zero-instruction
  `+S` constraint did keep the pointer in the target register and removed
  repeated GOT loads locally, but lowered the whole score to 36.144398%
  (13,979 bytes). Combining it with explicit `Player[0]`/`Player[1]` swap
  branches lowered the score further to 34.523346% (13,971 bytes). Both
  changes were reverted. The target's register lifetime comes from broader
  code layout, not a forced local pointer alone.
- A complete sweep of the 24 declaration orders for neutral, interactive,
  goody, and baddy counters at the three-slot frame layout produced only
  three whole-symbol scores: 37.911278%, 37.909122%, and 37.334410%, all at
  14,083 bytes. No order beat the retained neutral/interactive/goody/baddy
  spelling. Declaration order has a small GCC effect here but does not place
  the counters in the target's stack slots or fix the main-loop register map.
- At `17e058` the target tests `object->field_0xf00` directly after merging
  its awareness mask. Its later `AISysProcessCharacter` call also reads that
  byte for the final argument. The retained source instead carries an early
  `process_ai` boolean through the pad writes. Reassigning that boolean after
  awareness lowered 37.911278% to 37.307114%; direct reads at both use sites
  scored 37.226290%; volatile reads scored 37.222343%. Zero-instruction
  memory barriers around both direct reads recovered the target's local
  `test BYTE PTR [object+0xf00], 8` but scored 37.512930%; only the first
  barrier scored 37.332615%, only the second 37.293823%. The original
  register/block arrangement is still missing, so these trials were
  reverted. Removing the early local and spelling all three flag uses
  directly scored 37.157326%; GCC still combined the later reads. The
  target's separate reads remain a concrete alignment clue.
- In the route-toggle path, the target stores a literal zero to
  `input_toggle_hold_time` (`object+0xda4`) before route-state updates and
  later loads that field before subtracting `FRAMETIME`. The earlier build
  eliminated the zero store and carried the value in a register. A
  zero-instruction memory barrier after the source assignment restored the
  zero store and reload but lowered 37.911278% to 37.632904% (14,095 bytes).
  A volatile store produced the same operations at 37.630750%. Both were
  reverted. The original store is a clue to a different source-level alias
  or control-flow boundary; forcing it alone shifts later block alignment.
  Introducing a local timer selected from zero or the existing field before
  the subtraction compiled identically at the later 37.935345% baseline, so
  a named value alone does not preserve the missing store.
- The original reads the main-loop classification word at `object+0x1f4`
  fewer times than the 38.002514% build. Caching that word for both the
  collection tests and the later `0x40000` test reduced reads to the target
  count but lowered the whole score to 37.834053% (14,045 bytes). Caching it
  only for the collection tests compiled identically at 38.002514%, so the
  local was removed. Matching access counts alone does not fix the register
  lifetime or block order.
- In a mnemonic census of the retained build, the original has 233 `movss`
  and 114 `lea` instructions; the current function has 163 and 70. Thirty
  original-only stores of `xmm1` to `[esp+0x40]` and 26 original-only reloads
  dominate the scalar-move gap. The original first establishes a zero float
  in `xmm1`; the current build spills/recreates zero differently. An explicit
  `const f32 zero` used across the early phases generated identical code, so
  the live-range difference remains unexplained. Moving the existing
  zero-initialized `follow_offset` before the first object loop, then using it
  for that loop's zero visibility value, also compiled identically at
  26.952227%; GCC folds the source-level live range away.
- The later 37.798850% build has closed much of that earlier mnemonic gap:
  the original still has 233 `movss` and 114 `lea`, while the current caller
  now has 222 `movss` and 95 `lea`. Both sides emit about 30 stores of `xmm1`
  to `[esp+0x40]` (31 target, 30 current). The old zero-spill count is no
  longer the main lead; remaining alignment depends more on block order,
  register choice, branch targets, and the missing collision call.
- Most of the remaining `lea` count difference is alignment fill, not
  address arithmetic. In the 37.798850% build, the target has 48
  `lea esi, [esi]`, 10 `lea edi, [edi]`, and 32 one-byte `nop` instructions;
  the current caller has 35, 2, and 45 respectively. Their different block
  lengths change which multi-byte NOP form the assembler selects. Do not
  chase the raw `lea` count as a missing source operation.
- A full mnemonic census at 37.798850% shows the main directional
  differences after padding: target has seven more `cmovne` and five more
  `jns`; current has seven more `cmove` and five more `js`. Most other core
  mnemonic counts differ by only a few instructions. The remaining problem
  is largely branch polarity and register/stack allocation, not a missing
  large function call or a wholesale absent block.
- The original loads the player-array GOT entry once, while the current
  function loads it repeatedly. Forced pointer caching and a compiler barrier
  reduced matching. Treat the GOT-load count as a symptom of allocation and
  lifetime differences, not as a target to patch independently.
- A plain shared `player_slots = Player` local used by both eight-player
  scans also lowered 38.515804% to 36.387930% (14,173 bytes). Its build
  emitted 21 `GOT(Player)` loads versus 19 for the retained build. A C++
  alias alone did not extend the GOT pointer's machine-register lifetime;
  the direct `Player` accesses were restored.
- Ghidra shows the second player scan keeping `party_under_cover` in a
  local and storing it to the global after the scan. Rewriting the source
  that way lowered the 39.020832% build to 38.530174% (14,117 bytes),
  although the scan contains no calls that would distinguish the two
  forms semantically. GCC's register and stack allocation changed without
  reproducing the target loop handoff, so direct global updates remain.
- An isolated `-O2` override for `gameobjects.cpp` made `GameAIProcess` only
  10,682 bytes and lowered the GOT-aware score to 20.045618%. The normal
  `-O3` build is 13,549 bytes and 26.834412%. Keep the repository's `-O3`
  setting while investigating its source and translation-unit differences.
- The eight unrolled player checks update `all_under_cover` with seven
  `cmovne` instructions in the original. Before the retained `&=` change,
  the current code used seven `cmove` instructions and kept the accumulator
  in a register. A ternary, an
  explicit previous-value temporary, and `bool` storage did not recover the
  target instruction shape. The score stayed at or below 26.834412%.
  Repeating the exact `mask ? previous : 0` ternary after the later retained
  edits still emitted `cmove` on a register and scored 26.781970% versus
  26.952227% for bitwise `&=`.
  Hoisting each eligible player's `model_flags` before the context check
  dropped the score to 23.968750%; repeating that trial with the retained
  bitwise accumulator measured 23.931395%. Its conditional load order is
  important.
- At the 37.798850% baseline, a positive `bool all_under_cover` with a
  conditional keep-or-clear update still emitted seven register `cmove`
  instructions and scored 36.729168% (14,051 bytes). Marking that boolean
  `volatile` removed the conditional moves entirely, expanded the symbol to
  14,115 bytes, and scored 35.578663%. The original's seven `cmovne`
  instructions load a previous value from the stack and store it back;
  source-level polarity alone does not reproduce that register pressure.
- Caching `HIGHGAMEOBJECT` as a local bound and changing the first object loop
  from `<` to an explicitly guarded `!=` test lowered 26.663433% to
  22.530172%. Wrapping the loop body in a positive eligibility test instead
  of `continue` produced identical binary code. Neither source shape resolves
  the first loop's register and layout differences.
- The fresh 37.798850% decompile shows the target falls directly into the
  first object loop, while the current build initially jumps forward to its
  test and places the body before that test. Marking the loop continuation
  `__builtin_expect(index < HIGHGAMEOBJECT, 1)` caused a much wider GCC
  rewrite: 4.734195% and 13,145 bytes. That loop hint was reverted. The
  retained eligibility hint inside the loop is a separate, beneficial change.
- A guarded, explicit `do` loop with an equality exit and a `goto` for the
  eligibility skip also left the current build's initial jump-to-test
  layout intact. It lowered the GOT-aware whole-symbol match from
  38.002514% (14,117 bytes) to 34.725216% (14,117 bytes). GCC moved the
  loop body despite the source's body-first spelling; the original `for`
  loop was restored.
- Moving the `process_ai` flag assignment below the `0x40000` classification,
  and checking `field_0xf00` separately in that early-exit path, looked
  closer to the target decompiler's load placement. The whole-symbol match
  fell from 38.515804% (14,125 bytes) to 37.729527% (14,109 bytes).
  The shared early assignment was restored; the decompiler's apparent load
  placement did not explain GCC's register lifetime here.
- Forcing the early `process_ai` value through a `+m` stack slot freed
  `edx` for the classification flags, matching the target's local `mov
  edx, [object+0x1f4]` and `test dh, 4`. It lowered the full score from
  38.515804% to 37.495330% (14,139 bytes), because the extra stored flag
  and shifted register lifetimes disturbed the rest of the loop. The
  constraint was removed.
- Recomputing the `AISysProcessCharacter` flag argument from
  `object->field_0xf00 >> 3`, as the target's call setup suggests, lowered
  38.515804% to 38.299927% (14,141 bytes). A zero-instruction `+m`
  constraint on the field immediately before the call compiled identically
  to that trial. The shared `process_ai` argument was restored.
- The target's sign-bit check immediately after `AISysProcessCharacter` uses
  `jns` to the non-player path, whereas the 37.907690% build uses `js` to the
  player path. Hinting the non-player source condition as unlikely rewrote
  much more of the main loop and dropped the score to 32.395832% (14,036
  bytes). It was reverted. A matching branch polarity is not worth a broad
  block-layout regression.
- Repeating that non-player hint after the `field_0xef9` stack and sign-bit
  improvements again recovered the target's local `jns`, but lowered the
  whole-function score from 38.515804% (14,125 bytes) to 34.028736%
  (14,082 bytes). The larger control-flow change still dominates, so the
  hint was reverted.
- Rewriting the antinode timer exchange as the target decompiler's nested
  `<=` checks with shared copy labels lowered the 38.515804% build to
  38.094110% (14,139 bytes). Its local timer-copy block still used a
  different register and comparison order. The earlier compact exchange
  remains; the decompiler's source-like branch nesting did not recover
  the target's machine layout.
- On the later 46.595905% layout, a nested timer exchange using
  `!(timer > threshold)` to preserve the target's unordered path compiles
  identically to the compact exchange. Hinting the first fallback unlikely
  emits the target's local `jbe` but drops the full score to 45.766520%;
  hinting both fallbacks scores 44.936060%. Literal nested `<=` checks
  score 46.594110% and change NaN behavior. GCC canonicalizes the
  equivalent nested expression, so the compact source remains retained.
- Hinting the smaller `field_0xef9 & 0x80` branch as likely did recover the
  target's local `jns` and fallthrough copy, but the whole-function score fell
  from 37.907690% to 37.562500% (14,075 bytes). That hint was reverted as
  well. Compare the whole symbol after changing branch probabilities; a
  locally matched branch can disturb later block alignment.
- Hinting the `ai.field_0x1e7 & 0x80` collision-priority bit as likely also
  changed the current `js` to the target's `jns` and recovered the immediate
  fallthrough write. The global score nevertheless fell from 37.907690% to
  37.736350% at the same 14,083-byte size. The hint was removed. These two
  sign-bit trials show that local branch polarity is only one part of the
  register and stack match.
- After the later retained collision-priority hint, forcing the fallback
  X/Z distance scalars into addressable stack homes with input-only `m`
  constraints did emit two stores, but scheduled the first store before
  the second component's load and lowered 39.016884% to 38.067528%
  (14,133 bytes). The target stores both components after computing the
  radius. The constraints were removed; materializing the right slots
  also requires the target's computation order.
- Placing those two fallback components in one `NUVEC` and exposing its
  X/Z members as input-only memory operands recovered the target's
  `xmm3`/`xmm2` arithmetic and two adjacent stack stores, but the radius
  add moved after the stores and the whole match was 38.537357%
  (14,133 bytes) versus the 39.016884% baseline. A register-only no-op
  constraint on the radius compiled identically; adding a full memory
  clobber dropped the match to 37.096264% (14,035 bytes). The simple
  scalar source was restored.
- A nested collision-priority decision lowered 26.663433% to 24.721624%.
  That first trial reversed which object supplied each fixed flag, so it is
  not useful evidence for the target branch shape. A corrected nested trial
  following Ghidra's object mapping measured 24.894396% against a 26.834412%
  baseline. The decompiler's branch structure is evidence about machine
  control flow, but copying its C syntax did not recover the optimizer input.
- Caching `FRAMETIME` explicitly for the first object loop lowered 26.663433%
  to 26.032328%. GCC already hoisted its value; the new local changed the
  surrounding register allocation without fixing the zero-value spill.
- A stack-backed `mini_cut_cam` local declared before the first object loop
  lowered 39.020832% to 38.819324% (14,117 bytes). The target loads and
  spills `MiniCutCam` after confirming the loop bound is positive; the
  early local changed that load scheduling and did not remove the current
  initial jump-to-test layout. Direct global use was restored.
- Caching `TouchControlsActive` before the first loop loaded its byte too
  early, before the loop-bound check, and lowered 39.629670% to
  39.298134% (14,045 bytes). Adding an outer positive-bound guard moved
  that load to the target's position after the guard, but rewrote more of
  the loop and lowered the whole score to 35.730244% (14,061 bytes).
  Direct use of the global inside the loop remains retained; fixing one
  hoisted load in isolation did not preserve the target's block layout.
- Adding an explicit `"c"(high_flags)` operand beside the retained
  input-only memory operand did force the byte into `ecx`, as in the
  target's first-loop eligibility block, but lowered the whole score
  from 39.629670% to 39.098060% (14,085 bytes). The register pin was
  removed; the memory-only operand retains the better global layout.
- Copying `high_flags` into a `+c` constrained integer and masking that
  integer with `0x10` emitted the target's `and ecx, 0x10`, but also added
  a `movzx ecx, dl` and retained the original byte in `edx`. The whole
  match fell from 39.629670% to 39.483116% (14,029 bytes), so the
  register-copy form was removed. Matching an individual opcode does not
  recover the source register lifetime that produced it.
- Changing the first-loop index input constraint from `"d"` to `"+d"`
  on the recalibrated frame lowered 39.634340% to 39.434270% at the same
  14,029-byte size. The read/write constraint changes the index live
  range and hoisted-load scheduling without keeping the target's `edx`
  counter through the loop; the input-only constraint was restored.
- Constraining local pointers to `Obj` and `HIGHGAMEOBJECT` with `+D`
  and `+d` read/write operands lowered 39.636135% to 38.935703% and
  enlarged the function to 14,045 bytes. GCC inserted a move and stack
  spill for the count address, then chose a worse bound test and first-loop
  register allocation. The input-only preheader constraints were restored.
- Splitting the first-loop capability word into explicit old and masked
  locals makes GCC emit the target's `mov esi, ecx; and esi, 0xff1fffff`
  sequence and improves the first hundred aligned instructions from 26
  matches to 33. The whole score nevertheless falls from 39.636135% to
  39.586567% at 14,029 bytes. Adding a `+S` constraint on the masked word
  lowered it further to 39.155890% (14,045 bytes). Keeping the old word
  live through a late input-only constraint changed the final bit test to
  `test` but lowered it to 39.572197% (14,045 bytes). All three forms were
  removed. Register allocation in the larger function dominates this
  locally closer sequence.
- A guarded, addressable `touch_active` local after the first-loop bound
  test recovers the target's early `TouchControlsActive` GOT/load/spill
  order, but places the byte at `esp+0xe0` rather than the target's
  `esp+0x114`. With an explicit zero initialization it scored
  39.181034% (14,045 bytes); without one, 39.172413% (14,029 bytes),
  both below 39.636135%. The target's early load is intertwined with
  stack-slot allocation and loop layout; direct global use was restored.
- Hinting the first-loop third capability update (`& 0x20`) as unlikely
  changes the inline `je` to the target's distant `jne`, but lowers the
  whole score to 39.048850% (14,046 bytes). Combining that hint with
  explicit old/masked capability locals scores 39.098060% (14,046 bytes),
  also below the 39.636135% baseline. The branch polarity matches locally,
  but the moved block perturbs later alignment; both forms were removed.
- Repeating that third-capability unlikely hint after the retained
  action-frame hint lowers 40.008263% to 39.183907% at the same
  14,035-byte size. The better nearby action-frame layout does not make
  that distant-branch placement beneficial; it was again removed.
- Wrapping the first `for` loop in an explicit `HIGHGAMEOBJECT > 0`
  guard on the 40.008263% layout lowered the score to 39.608837% at
  14,035 bytes. GCC still emitted an initial jump into the loop test,
  despite the positive outer guard, and changed the bound's register and
  preheader stack store. The guard was removed.
- Retesting the first-loop index as a `+d` read/write operand after the
  retained action-frame hint lowered 40.008263% to 39.937500%, with the
  function still 14,035 bytes. The action-frame byte still lived in
  `ecx`, so this did not free `esi` for the target's decrement sequence;
  the input-only index constraint was restored.
- On the later 40.320763% latch/body index layout, removing both
  preheader GOT-order constraints lowers the score to 40.279810%; keeping
  only the two-address constraint scores 40.318966%. The second pointer
  input still adds a small gain, so both were retained. These tiny
  preheader wins remain sensitive to loop register allocation.
- A 64-byte output-only addressable pad next to the hoisted `high_flags`
  local grows the frame from `0x570` to `0x5b0` but leaves all first-loop
  scalar stack homes at the same offsets (`FRAMETIME` pointer at `0xb8`,
  flags byte at `0xc8`, touch byte at `0xd4`). It scores 40.678880%,
  equal to the uncalibrated hoist and below 40.682114%. Declaring the pad
  before or after `high_flags` compiles identically. Added frame space
  alone does not shift this scratch cluster toward the target's offsets.
- Caching the first loop's `FRAMETIME` value in a scoped `f32` local
  lowered the 40.682114% build to 39.310345% (14,051 bytes) and shrank
  the frame to `0x560`. It moved the bound and zero-vector setup before
  the target's early touch/frame-time sequence instead of forcing the
  target's `xmm2` choice. Direct global reads were restored.
- Recomputing `process_ai` from `field_0xf00` after the field-scoped
  barrier, then using the later value for the update block and
  `AISysProcessCharacter` argument, lowered 40.700430% to 39.779095%
  and reduced the function from 14,091 to 14,043 bytes. The target does
  have later field tests, but changing the carried decision also changes
  the larger main-loop register flow. The separate later local was removed.
- Guarding the addressable `MiniCutCam` load with `HIGHGAMEOBJECT > 0`
  moves its GOT/load/spill triplet after the target-like positive bound
  test but lowers 40.904095% to 40.644756% at 14,091 bytes. The guard
  changes subsequent register scheduling despite improving this local
  ordering; the preloop load was restored.
- Caching `GAMEPAD_START` in an addressable preloop integer after the
  retained `MiniCutCam` cache lowers 40.904095% to 40.437140% and grows
  the function from 14,091 to 14,107 bytes. It duplicates the button
  mask's stack store and loads the value before the positive bound test,
  unlike the target. Direct global use was restored.
- Caching `TouchControlsActive` in an addressable preloop byte on the
  40.904095% layout lowers the score to 40.590878% (14,075 bytes).
  GCC emits an extra touch-byte stack store and shifts the bound/zero
  setup before the target's sequence. Direct global use was restored.
- Swapping the declaration order of the retained `MiniCutCam` integer
  and hoisted `high_flags` byte compiled identically at 40.904095% and
  14,091 bytes. Their stack slots are not controlled by this source
  declaration order; the original order was restored.
- Retesting the non-player branch hint after `AISysProcessCharacter` on
  the 40.904095% layout again changes the local `js` toward the target's
  `jns`, but drops whole-symbol matching to 36.046696% (14,034 bytes).
  This third trial confirms that source-level probability alone moves too
  much of the main-loop control flow; the hint was removed.
- Constraining the incremented main-loop `GameObject_s *` to `edi` at
  the loop latch, instead of at body entry as in an earlier trial,
  lowers 40.904095% to 39.095547% and shrinks the function from
  14,091 to 13,925 bytes. It changes the loop's register lifetimes too
  broadly to recover the target's object-pointer register; the normal
  pointer increment was restored.
- A read/write memory constraint on the 64-bit awareness accumulator
  immediately after its main-loop OR lowered 40.904095% to 40.355602%
  and shortened the function to 14,043 bytes. GCC emitted direct
  `or [esp+0xe0/e4], eax` updates, whereas the target loads each half,
  ORs it in a register, and stores it at `esp+0xf0/f4`. The field-only
  constraint was removed; addressability alone selects the wrong form.
- Splitting awareness into two `volatile u32` halves emitted the target's
  load/OR/store shape at the main-loop entry, but scored 40.450430%
  (14,059 bytes, `0x560` frame). Restoring the frame with a 48-byte
  placeholder only raised it to 40.452587%. Removing `volatile` from
  the halves scored 40.545980% (14,091 bytes, `0x580` frame). All three
  remained below the retained 40.904095% 64-bit accumulator, so the
  original scalar and 32-byte placeholder were restored.
- Retesting the first-loop third capability branch as unlikely after the
  hoisted high-byte and cached `MiniCutCam` changes lowers 40.904095%
  to 39.378952% and grows the function to 14,115 bytes. Even with the
  closer nearby `esi` capability instructions, moving the uncommon block
  damages whole-function layout; the hint was removed.
- A 64-byte output-only pad at the very start of `GameAIProcess` grows
  the frame from `0x570` to `0x5b0` but leaves the first-loop scratch
  homes unchanged (`FRAMETIME` pointer `esp+0xb8`, high flags `esp+0xc8`,
  touch byte `esp+0xd4`). It scores 40.900864% versus 40.904095% and
  shifts the already matched pointer arrays. Moving extra frame space
  earlier in source still does not move this scalar allocation group;
  the pad was removed.
- Replacing the first player scan's inverse `any_uncovered` accumulator
  with a positive `all_under_cover` value lowered 40.904095% to
  40.604527% at 14,091 bytes. GCC still emitted register `cmove` rather
  than the target's stack-backed `cmovne`. A `+m` constraint after each
  update did emit `cmovne`, but grew the frame to `0x5a0`, enlarged the
  function to 14,195 bytes, and lowered the whole score to 36.803520%.
  The inverse accumulator was restored. Matching seven local conditional
  moves is insufficient when the extra addressable lifetime shifts the
  rest of the stack and control flow.
- An explicit first-loop pointer to `FRAMETIME` with an input-only memory
  operand lowered 40.904095% to 39.512573% (14,067 bytes, `0x560`
  frame). GCC loaded the global GOT address before the positive bound
  test and again afterward, rather than reusing the target's single
  stack-backed pointer. Without the memory operand, the pointer alias
  compiled identically to direct global reads at 40.904095%; the alias
  was removed.
- Moving the zero-initialized `follow_offset` before the first object
  loop and using it for that loop's zero visibility value again compiles
  identically at 40.904095% and 14,091 bytes. GCC folds the apparent
  cross-loop floating-point lifetime even on the newer layout; the
  declaration was returned to its later scope.
- Giving the first player scan its own `Player` pointer and constraining
  that pointer with read/write `+S` makes GCC load the GOT address once
  and keep it in `esi`, as the target does. With the retained inverse
  cover accumulator, however, the whole score falls from 40.904095% to
  40.316810% and size rises from 14,091 to 14,243 bytes. An input-only
  `"S"` constraint scores 39.447197% (14,123 bytes). Combining `+S`
  with a positive cover accumulator emits the target's seven `cmovne`
  instructions with stack source, but at `esp+0xc8` rather than target
  `esp+0x104`; the whole score is 40.211210% (14,227 bytes). The input-
  only pointer form with positive cover emits no `cmovne` and scores
  39.450430%. A 60-byte addressable pad beside the cover accumulator
  grows the frame to `0x5b0` without moving its `0xc8` slot and scores
  40.207973%. These source forms capture two target local patterns, but
  their extra register pressure and unresolved stack placement reduce the
  overall match, so the 40.904095% source was restored.
- The target has three `TestWalkAround` call sites near the function's
  cold tail; the retained build has two aligned sites and merges the
  third source call. On the 40.904095% layout, an empty asm after the
  first or second source call restores three sites but moves one emitted
  call into the main body around aligned index 1206; scores are
  40.601654% and 40.661278% (14,115 bytes). A third-branch input-only
  memory operand still leaves two sites and scores 40.851654%; a full
  memory clobber after that call also leaves two and scores 40.901940%.
  All markers were removed. The target's third call needs distinct
  surrounding data flow, not merely a compiler barrier after the call.
- Changing the preloop `MiniCutCam` local's constraint from input-only
  `m` to read/write `+m` forces a reload, but schedules it before the
  target's touch/visibility sequence, enlarges the frame to `0x580`,
  and lowers 40.904095% to 40.644398% (14,107 bytes). Keeping the
  input-only spill and placing `+m` immediately before the source use
  puts the reload at the target's local instruction position, but lowers
  the whole score to 40.325790% (14,067 bytes). The first-loop stack
  home is still `esp+0xec`, versus target `esp+0x118`; both reload
  variants were removed.
- Constraining the first-loop `Obj` GOT pointer to `edi` makes the first
  and third preheader instructions match, but lowers 40.904095% to
  40.201508%. Pinning the `HIGHGAMEOBJECT` GOT address to `edx` as well
  makes the first three instructions match but duplicates the count GOT
  load after the early `MiniCutCam` load and scores 39.966953% (14,107
  bytes). Guarding the camera load then puts the bound test in the target
  order, but scores 40.579384%. Caching the bound in a local constrained
  to `edi` emits an extra `mov edi, ecx` and scores 40.568966%. The
  original input-only preheader constraints, unguarded camera cache, and
  global loop bound were restored. Register identity at entry is not
  sufficient without matching the target's load schedule and lifetime.
- Caching the main-loop `pad->pad` pointer in a local constrained to
  `edx` recovers the target's `mov edx, [eax]; test edx, edx` pair in
  place of the current memory `cmp`, but lowers whole-symbol matching
  from 40.904095% to 39.686424% (14,089 bytes). Because the function
  jumps to its interaction label over this source site, the temporary
  must be declared before that `goto` and assigned at the pad check.
  The extra register lifetime changed the larger loop, so the local
  and constraint were removed.
- The target's two negated collision paths use different pre-call
  argument/register setup before their separate `TestWalkAround` calls.
  In the retained build, the current main-loop object lives in `esi`
  where the target's lives in `edi`; this contributes to GCC merging
  their identical call tails. Constraining `interactive_cursor` to `esi`
  at main-loop entry in an attempt to free `edi` for the object lowered
  40.904095% to 37.670980% (14,099 bytes) and disturbed an earlier
  opponent-selection call block. The cursor constraint was removed.
- Shadowing the first-loop `object` variable with a separately scoped
  `GameObject_s *object` for the main loop compiles identically at
  40.904095% and 14,091 bytes. GCC's SSA lifetime is unaffected by
  that source scope because the pointer is reset to `Obj` before the
  second loop; the simpler existing variable was restored.
- Hoisting the avoidance-loop `NUVEC difference` from the inner pair
  loop to the outer avoidance block, then as far as the function's main
  vector declarations, compiled identically at 40.904095% and 14,091
  bytes. The compiler keeps its current `esp+0x110/0x118` slot and still
  merges two `TestWalkAround` call tails. Source scope alone does not
  extend the vector's machine lifetime; the declaration was returned
  to the inner loop.
- Keeping a separate 64-byte output-only stack array live across the
  avoidance loop enlarged the frame from `0x570` to `0x5b0`, but the
  collision vector stayed at `esp+0x110/0x118` instead of the target's
  `esp+0x160/0x168`. The linked-GOT score fell from 40.904095% to
  40.897270% at the same 14,091-byte text size. A live array and a larger
  frame do not by themselves move an unrelated local; the probe was removed.
- Changing the `field_0xf00` read/write memory constraint to input-only
  compiled identically at 40.916310% and 14,083 bytes. Changing the
  staged `field_0xef9` byte constraints one at a time from read/write to
  input-only lowered the score to 40.655533% for `shifted_flag`,
  40.687860% for the first `cleared_flag`, and 40.738148% for the final
  `cleared_flag`. The byte constraints were restored. Empty asm operand
  direction must be measured separately for each data flow.
- Forcing the old route suit index into an addressable local before
  `Suit_GetIndex` reproduced the target's pre-call spill and post-call
  comparison, but grew the frame to `0x580` and scored 40.824352%
  (14,091 bytes). Nesting the spill under the character-ID equality check
  scored 40.868176% (14,083 bytes); its store still used `esp+0xf0`
  instead of target `esp+0x100`. Both forms were removed. Local spill
  resemblance does not outweigh frame and control-flow changes elsewhere.
- Adding an empty post-call operand to keep `first` live after the third
  collision `TestWalkAround` still emitted two helper calls and lowered
  40.916310% to 40.698277% (14,106 bytes). Reading the second object's
  priority as a memory operand after that call also left two call sites
  and scored 40.880750% (14,083 bytes). Adding a memory operand after the
  first call emitted three sites, but moved one to aligned instruction
  1206 rather than the target's late collision region and scored
  40.508620% (14,122 bytes). Call count alone is insufficient; the
  compiler's basic-block placement must also match. All three probes
  were removed.
- Pinning the main-loop object to `edi` inside the interactive-object
  collection path with a read/write constraint lowered 40.916310% to
  36.406967% (14,163 bytes, `0x590` frame). An input-only `edi`
  constraint scored 39.258263% (13,925 bytes, `0x560` frame). Both
  changed stack allocation and the whole loop much more than the local
  object register; neither was retained.
- Replacing the post-`AISysProcessCharacter` non-player `if` with a
  branch to an equivalent label compiled identically at 40.916310%.
  Moving the full non-player source block after the common FreePlay block
  and jumping back preserved execution order, but GCC still emitted the
  non-player path as fallthrough (`js` to FreePlay) and scored 39.411636%
  (14,179 bytes). Marking non-player unlikely in that layout emitted the
  target's local `jns` polarity but scored only 35.994250% (14,114 bytes).
  Source block order and one likelihood hint do not recover the target's
  larger post-process block arrangement; both variants were reverted.
- Assigning an early boolean for `AISysUpdateCharacterPathPos` and reloading
  the raw AI-update byte after the awareness merge moved its load closer
  to the target block, but lowered the 41.043102% raw-byte build to
  40.582615% (14,051 bytes). Compound shifting the `u8` local at the
  call compiled identically to a direct `>> 3` expression; GCC still
  used `shr eax, 3` rather than the target's `shr al, 3`. A `+a` empty
  asm constraint on that byte added redundant register moves and scored
  40.938576% (14,115 bytes). These variants were removed.
- Retesting a read/write memory constraint on the 64-bit awareness
  accumulator after retaining the raw AI-update byte scored 40.783405%
  (14,051 bytes), below the 41.136494% current layout. The raw byte
  change did not make the earlier awareness-spill experiment profitable;
  the constraint was removed.
- An explicit `shr` on the raw `u8` AI-update byte with an `al`
  read/write operand did emit the target's `shr al, 3`, but GCC scheduled
  it before the argument setup and added two register moves. The score
  dropped from 41.136494% to 40.715157% (14,115 bytes). The explicit
  instruction was removed; matching an opcode without its surrounding
  data flow is a regression.
- Moving the retained raw AI-update byte load from before the main-loop
  classification to just after its early `0x400` branch, with a direct
  field test in that branch, lowered 41.136494% to 40.635060% (14,067
  bytes). Reloading it after the awareness merge had already scored
  40.582615%. The early load and carried byte give GCC a more favorable
  register lifetime even though the target reloads closer to the later
  test; the original placement was restored.
- Inverting the first player-cover accumulator from `any_uncovered` to
  `all_under_cover` on the 41.136494% layout scored 40.818966%
  (14,099 bytes) and still emitted eight `cmove` and no `cmovne`.
  Making that positive accumulator addressable scored 40.584770%
  (14,099 bytes, `0x580` frame) and emitted nine `cmove`. Both changes
  were removed. Accumulator polarity or stack addressability alone does
  not recover the target's seven `cmovne` selections.
- Hinting the non-player path after `AISysProcessCharacter` as unlikely
  together with `FreePlay != 0` as likely changed the local fallthrough
  paths but lowered 41.136494% to 34.556034% (14,056 bytes). Hinting
  only the FreePlay test scored 39.911278% (14,125 bytes). The hints
  reordered large parts of the function and were removed.
- Replacing only the main-loop interactive-object cursor store with an
  indexed `interactive_objects[interactive_count++]` store scored
  40.104168% (14,099 bytes) on the 41.136494% raw-byte layout. Replacing
  both collection sites with indexed stores scored 39.404810% (14,067
  bytes). The main object still lived in `esi`, so removing the cursor did
  not free `edi` for it. Both sites were restored to cursor increments.
- Putting the positive cover accumulator inside a containing struct with
  a 60-byte leading field, together with the first-scan `Player` pointer
  constrained in `esi`, retained seven target-like `cmovne` instructions
  but placed the accumulator at `esp+0x1ac` in a `0x5b0` frame. A 12-byte
  leading field placed it at `esp+0x15c` in a `0x580` frame. Both scored
  40.394756% (14,275 bytes), below the retained 41.136494%, and neither
  approached the target `esp+0x104` slot. An aggregate's allocation region
  dominates the explicit internal offset; the containing struct was removed.
- On the 41.224857% layout, indexing the main-loop neutral-object store
  as well lowered the score to 41.121770% (14,099 bytes); the neutral
  cursor remains. Removing the now earlier-scan-only goody cursor and
  indexing that scan too scored 41.082973% (14,067 bytes). The goody
  cursor was restored only for the player scan. Cursor/index choices are
  sensitive to the phase's live ranges, not just the list's element type.
- Rechecking the main-loop interactive store as indexed after the goody
  change still lowered 41.224857% to 40.192528% (14,099 bytes); the
  interactive cursor remains. Removing the earlier player-scan neutral
  cursor after the baddy/neutral improvement lowered 41.309628% to
  36.582973% (14,044 bytes), so that cursor also remains. Replacing a
  cursor in one phase does not imply it can be removed from the other.
- After the baddy pointer and indexed main-loop neutral store, removing
  the remaining player-scan goody cursor scored 41.225933% (14,099 bytes,
  `0x580` frame) versus retained 41.309628%. The goody cursor was
  restored for that scan.
- Giving the collision `NUVEC difference` a function-long addressable
  lifetime, with an output memory operand near list setup and an input
  operand after avoidance, moved its slot from `esp+0x100` down to
  `esp+0xe0`, away from the target's `esp+0x160`. It retained two
  `TestWalkAround` call sites and scored 41.306034% (14,131 bytes),
  slightly below 41.309628%. The local was returned to the inner pair
  loop. Forcing lifetime is distinct from merely hoisting scope, but it
  still does not place this vector at the target offset.
- Repeating the empty marker after the first collision helper call on
  the 41.309628% list-layout build emitted a third call, but again moved
  one to the hot aligned region near instruction 1205 rather than the
  target's cold collision tail. The score fell to 40.999640% (14,163
  bytes). The marker was removed. The new list layout does not change
  that cross-jumping/block-placement pattern.
- Moving the loop-long `baddy_cursor` initialization into its main-loop
  `0x400` classification branch shortened its lifetime but lowered
  41.309628% to 39.456177% (14,123 bytes, `0x580` frame). The pointer
  needs the earlier lifetime for the current register layout. Swapping
  its declaration order with `interactive_cursor` compiled identically;
  the original order was restored.
- Swapping the two nested fixed-second collision branches compiled
  identically at 41.309628% and 14,131 bytes, so source ordering alone
  does not change GCC's cold-block order. Flattening the three call
  conditions with the non-negated path first moved a call to the hot
  aligned region near instruction 1219 and scored 40.481323% (14,177
  bytes); the nested conditions were restored. Moving the final negated
  path's priority store after vector negation also compiled identically.
- Hinting the first negated collision path (`!fixed_second`) unlikely
  retained two call sites and scored 41.307472% (14,131 bytes), just below
  baseline. Hinting it likely scored 40.140804% (14,149 bytes) and changed
  the surrounding cold layout. Neither hint recovered the target's three
  separate calls; both were removed.
- With the outer `!fixed_first` likely hint, marking that condition
  unlikely instead scored 41.092674% (14,141 bytes) against the unmarked
  41.309628% baseline; the likely hint scored 41.357040% (14,128
  bytes). Adding an inner `!fixed_second` unlikely hint to the retained
  outer hint compiled identically, so it was removed. Pinning the
  collision pair's `first` pointer to `edi` at outer-loop entry lowered
  the new 41.664154% build to 36.153020% (14,015 bytes), despite making
  the target register available near the calls. That constraint was
  removed; pointer register identity still depends on broader lifetimes.
- Keeping the outer likely hint but using an input-only memory operand
  after the first negated call scored 41.504670% (14,173 bytes), below the
  41.664154% empty-marker form. Moving the empty marker to only the third
  call scored 41.355244% (14,128 bytes) and emitted two sites. Keeping
  markers after both the first and non-negated second calls likewise
  scored 41.357040% (14,128 bytes) with two sites. Only the second-call
  marker is retained at 41.723778%.
- Inverting the inner fixed-second branch while keeping that second-call
  marker compiled identically at 41.723778% (14,166 bytes). Hinting its
  resolved-priority comparison likely lowered the score to 41.386494%
  (14,160 bytes); hinting it unlikely compiled identically. Hints on the
  third call's resolved-priority comparison, either likely or unlikely,
  compiled identically. The simpler unhinted nesting is retained.
- Reusing each main-loop `NUVEC` as the avoidance-pair `difference` via a
  reference changed the stack slot but did not reach the target's
  `esp+0x160`: `wall_direction` placed it at `esp+0xe0`, `lateral`,
  `direction`, and `forward` at `esp+0xf0`, and `ray` and `movement` at
  `esp+0x100`. Each scored 41.720547% (14,166 bytes), slightly below
  the separate local at 41.723778%. The target reuses `esp+0x160` in
  several earlier blocks, whereas the current build uses `esp+0x100`
  for the collision vector. Source-level vector reuse is insufficient
  to reproduce the target stack coloring, so the local was restored.
- Hinting the avoidance-pair `distance < radius` guard unlikely scored
  41.270473% (14,163 bytes), and hinting it likely scored 40.871770%
  (14,172 bytes), both below the unhinted 41.723778% build. This guard
  affects placement of the whole collision body; the unhinted form is
  retained.
- Forcing the first player-scan's `any_uncovered` to a stack slot only
  after the scan with an empty read/write memory operand scored 39.965520%
  (14,142 bytes); an input-only operand scored 40.302440% (14,126 bytes)
  and still tested the register. Both lowered the 41.723778% layout.
  The target's stack-backed cover decision at the scan exit cannot be
  recovered just by making the final value addressable.
- Pinning only the first scan's `Player` pointer to `esi` with a read/write
  register operand recovered that GOT load but scored 41.234913%
  (14,318 bytes). Combining it with a positive `all_under_cover`
  accumulator produced seven target-like `cmovne` instructions yet
  scored 39.858480% (14,302 bytes). The pointer and accumulator
  constraints were removed; matching these local instructions changed
  too much code elsewhere.
- Carrying one `baddy_cursor` from the player scan into the main object
  loop instead of starting it at `baddies + baddy_count` after the scan
  scored 41.696120% (14,166 bytes, `0x570` frame), just below the
  41.723778% retained layout. It changed the scan-to-main-loop handoff
  but left the main object in `esi`; the shorter-lived cursor was restored.
- A weaker input-only `"D"` constraint on the main-loop object just before
  the loop scored 35.990660% (14,088 bytes). GCC used `edi` for the loop
  counter and still kept the object in `esi`, unlike the target, then
  changed many downstream blocks. The constraint was removed. Merely
  requesting `edi` once at loop entry does not pin the intended live range.
- On the retained `any_uncovered == 0` unlikely layout, hinting the adjacent
  `party_cant_be_under_cover == 0` condition either way compiled identically
  at 42.579020% (14,175 bytes). Hinting only `VADER_ADATA == NULL` likely
  lowered the score to 41.527657% (14,183 bytes), while unlikely compiled
  identically. Hinting the full `VADER_ADATA`/area disjunction either way
  also compiled identically. The cover hint has a distinct effect that
  cannot be reproduced by generic hints on its neighbors.
- Hinting the first-scan cover update's model-flag test likely or unlikely
  compiled identically at 42.579020% (14,175 bytes). GCC lowers that
  ternary to conditional moves, so this likelihood hint did not alter
  the unrolled scan; the unhinted update is retained.
- On the direct early `field_0xf00` memory-test layout, moving the raw
  byte load after the awareness merge produced a later `movzx` near the
  target's instruction 871, but the whole match fell to 42.133620%
  (14,095 bytes) from 42.639730%. The early load and empty register use
  were restored. Matching the later load location alone shortens the
  byte's live range too much for the surrounding layout.
- Forcing the early AI-update byte into `eax` with a read/write `+a`
  operand freed `edx` for the classification flags, recovering the
  target's `test dh, 4` near the main-loop entry. It nevertheless fell
  to 41.895473% (14,111 bytes) from 42.639730%. Input-only `"a"`
  avoided the broad change but scored 42.625360% (14,175 bytes) and
  left the flags in `eax`. Both operands were removed; this is another
  case where one target register is not enough to match global lifetimes.
- With both main-loop goody stores using their cursor, changing the
  main-loop neutral store to its cursor scored 42.161278% with a
  `0x580` frame. Reducing stack padding from 32 to 16 bytes restored
  the `0x570` frame but scored only 42.164513%, below the indexed
  neutral store's 42.690730%. The neutral cursor remains confined to
  the earlier player scan; matching frame size cannot recover the
  register and block changes caused by this longer cursor lifetime.
  When reverting, patch each occurrence with its surrounding branch
  context: an accidental opposite mix (indexed in the player scan,
  cursor in the main loop) measured 42.777657% but overwrites prior
  neutral entries and is invalid. Its score must not be retained.
- Caching `object->apiobj.field_0x1f4` across the initial `0x400`
  classification and subsequent `0x40000` test recovered the target's
  `edx`-based `test dh, 4` and `and edx, 0x40000`, but scored 42.477370%
  (14,087 bytes) because the early AI-update byte spilled. Moving that
  byte load after the awareness merge removed the spill and scored
  42.646190% (14,071 bytes), still below the retained 42.690730%.
  The explicit classification local and late byte load were removed;
  local register/instruction matches do not offset the changed broader
  live ranges and block alignment.
- The wall-shuffle body begins near aligned instruction 1999 in the
  target but near 957 in the retained build, leaving a 292-instruction
  nonmatching span. Hinting its traversal-mask test unlikely moved
  that body to roughly 3278 and scored 36.770832% (14,122 bytes);
  likely kept it near 949 and scored 36.449352% (14,249 bytes).
  Hinting only `connection != NULL` unlikely likewise moved it near
  3281 and scored 36.123924% (14,141 bytes); likely kept it near 957
  and scored 41.816452% (14,197 bytes). The target's intermediate
  placement likely depends on surrounding block order or live ranges,
  not a single likelihood hint on this guard. All hints were removed.
- Explicitly inverting the wall-shuffle condition and routing the
  alternatives through `goto` labels compiled identically to the
  42.690730% unhinted source (14,175 bytes). GCC canonicalized the
  equivalent control flow, so the simpler `if`/`else if` was restored.
  With the later retained non-player-path likely hint, adding an inner
  wall-shuffle mask hint still regressed: unlikely scored 38.060345%
  (14,122 bytes) with the body near instruction 3244, and likely scored
  37.285920% (14,211 bytes) with it near 956. The outer hint remains
  without an inner hint.
- On the retained non-player-path likely layout, hinting the following
  `FreePlay != 0` condition likely scored 42.574710% (14,203 bytes),
  and unlikely scored 41.829742% (14,167 bytes), both below the
  unhinted 43.481323%. Neither moved wall-shuffle out of its early
  block; the FreePlay test was restored.
- With the stuck-path state check marked unlikely, hinting either the
  following path flag or the floating-point stuck-time comparison in
  either direction compiled identically at 43.532690% (14,186 bytes).
  Those four redundant hints were removed; only the state check changes
  the current control-flow layout.
- Combining the retained stuck-state unlikely hint with a wall-shuffle
  mask unlikely hint still sent wall-shuffle to the far tail (near aligned
  instruction 3128) and scored 36.703663% (14,100 bytes). The mask hint
  was removed. Its extreme block-placement effect persists across the
  neighboring stuck-path change.
- After `AISysProcessCharacter`, an explicit skip label with the signed
  flag test marked likely made GCC emit the target's `jns` branch toward
  non-player work, but moved wall-shuffle to instruction 3160 and scored
  36.116380% (14,102 bytes). Marking that skip unlikely compiled
  identically to the retained 43.532690% source with `js` toward FreePlay
  (14,186 bytes). Branch polarity alone does not place the large
  non-player blocks correctly; the simpler hinted `if` was restored.
- Hinting the main-loop `field_0x1f4 & 0x40000` interactive-class branch
  likely lowered 43.532690% to 24.156970% (14,294 bytes), and hinting
  it unlikely scored 29.437141% (14,236 bytes). This guard controls a
  large region of the function; either explicit probability radically
  changes GCC's block order. It remains unhinted.
- Hinting the second player-scan `party_under_cover != 0` test likely
  scored 43.327587% (14,186 bytes), and unlikely scored 40.119250%
  (14,197 bytes), below the 43.532690% unhinted layout. The accumulator
  test participates in the scan's conditional-move setup; its current
  branch layout is more stable without a probability hint.
- With the retained positive cover accumulator and explicit zeroing
  branch, changing the final cover-decision hint from unlikely to likely
  scored 43.041668% (14,189 bytes); removing the hint scored 43.778020%
  (14,189 bytes), both below the unlikely 43.979527% layout. The hint
  remains attached to only that final accumulator test.
- On the later 43.979527% positive-cover layout, pinning only the first
  scan's `Player` pointer to `esi` with a read/write operand recovered
  seven `cmovne eax, [esp+0xd8]` instructions but scored 43.323635%
  (14,322 bytes). The target uses `[esp+0x104]` and places those
  instructions 26 aligned positions earlier. Making the accumulator
  addressable at initialization moved its source slot to `esp+0xbc`,
  added another conditional move, grew the frame to `0x580`, and scored
  42.134697% (14,334 bytes). Both constraints were removed. Local
  mnemonic recovery still disrupts the scan-to-main-loop handoff.
- Typing the retained positive cover accumulator as `bool` and writing
  `true`/`false` compiled identically to its `i32` zero/one form at
  43.979527% (14,186 bytes, `0x570` frame). GCC canonicalizes this
  local's boolean value on the current layout; the integer spelling
  was kept for consistency with the other scan counters.
- Rewriting the first object loop's eligibility check as a positive
  enclosing block, and separately rewriting the loop as `while` with an
  explicit latch label, both compiled identically to the 43.979527%
  source (14,186 bytes). GCC canonicalizes these loop forms here; a
  different spelling alone does not recover the target's body-first
  entry or register-held bound.
- Caching the first-loop `HIGHGAMEOBJECT` bound and constraining it to
  `edi` on the 43.979527% layout lowered the match to 43.000717%
  (14,202 bytes) and grew the frame to `0x580`. GCC inserted a stack
  spill and still tested a reloaded `esi` instead of the target's
  `edi`. The direct global bound was restored.
- After retaining the alert-expiry hint, marking the third touch
  capability bit (`0x20`) unlikely recovered the target's long `jne`
  locally but lowered the whole score from 44.953304% to 41.836926%
  (14,208 bytes). The branch was left unhinted. Carrying an explicit
  pre-mask capabilities word through all three touch-bit checks scored
  44.844826% (14,218 bytes), also below the retained layout, and was
  restored. Local opcode agreement still trades off against register
  lifetimes and cold-block ordering.
- Replacing the first loop's `< HIGHGAMEOBJECT` condition with a positive
  guard and `index != object_limit` recovered the target's `jne` latch,
  but scored 42.601290% (14,218 bytes) against the 44.953304% baseline.
  GCC compared the counter with a spilled bound in reverse operand order
  and changed the preheader and scan register lifetimes. Moving the cached
  `MiniCutCam` load inside that guard scored 42.592674% with a `0x580`
  frame. Pinning the guarded bound to `edi` then scored 42.403020% with a
  `0x590` frame. All three variants were removed. The target's initial
  `jle` and later `jne` are evidence for a guarded equality loop, but
  matching only those branches is insufficient on this layout.
- On the 45.610270% layout, marking the collision pair's outer
  `!fixed_first` test unlikely recovered the target's `je` at that site,
  but scored 45.225933% (14,192 bytes). Removing the hint scored
  45.196840% (14,191 bytes). The retained likely hint preserves the
  better wider block placement. Recasting the first fixed flag as a
  signed-byte negativity test compiled identically at 45.610270%; swapping
  the two fixed-flag declaration orders scored 45.526222% (14,206 bytes).
  All four probes were restored before the cover ternary retest.
- With the 45.653020% cover ternary, pinning the first scan's `Player`
  pointer to `esi` again recovered seven target-like `cmovne` selections,
  but scored 44.806750% (14,326 bytes) versus 45.653020%. Combining that
  pointer pin with the four-address preheader constraint scored
  44.791310% (14,326 bytes). The pointer pin was removed; the preheader
  constraint alone improves the whole match.
- Reversing the `party_under_cover` and `active_neutral_count` zero-store
  source order under the preheader constraint scored 44.248560%
  (14,210 bytes), despite the target storing cover first. Adding an
  input-only `Player` address constraint between the stores scored
  45.517960% (14,206 bytes) against the 45.778378% retained form.
  Those changes disturbed the subsequent global-load ordering and were
  removed.
- On the 45.778378% preheader layout, adding a fifth input-register
  address for `party_cant_be_under_cover` scored 45.516163% (14,206
  bytes). GCC loaded the address early, then reloaded it after losing the
  register, so the four-address constraint remains. Making the positive
  cover accumulator addressable at initialization scored 44.994250%
  (14,222 bytes) with a `0x580` frame and a slot at `esp+0x100`, near
  the target's `esp+0x104`. Reducing the scratch pad to restore the
  `0x570` frame scored 44.997486%; it did not move that slot or recover
  `cmovne`. Both changes were removed. An input-only memory operand on
  `active_neutral_count` after its zero store scored 45.731323%, also
  below the retained form.
- Precomputing `baddies + baddy_count` as the destination of the
  neutral-to-baddie append loop recovered the target's `jg` loop latch,
  but shrank the frame to `0x560` and scored 44.605602% (14,221 bytes).
  Enlarging the scratch pad restored the `0x570` frame and scored only
  44.608480%. Forcing the neutral source pointer into an addressable
  local instead scored 40.600574% (14,141 bytes). The source and
  destination register lifetimes matter more than the isolated latch
  condition; the original indexed append was restored.
- With the 45.778378% layout, forcing an input-register use of each
  first-scan `Player[index]` object immediately after its load scored
  45.769398% (14,206 bytes) and did not move the repeated `Player` GOT
  loads into the target's single carried `esi` pointer. Moving that use
  after the follow-offset store extended the object lifetime, shrank the
  function to 14,174 bytes, and scored 44.346622%. Both were removed.
  Changing the cached `MiniCutCam` local's input-only memory operand to
  read/write forced an early reload and grew the frame to `0x580`; it
  scored 45.640446% (14,206 bytes) against the retained 45.778378%.
  The input-only operand was restored. Forcing one target-like reload
  remains less useful than preserving the wider first-loop allocation.
- A single-hint sweep on the 45.778378% layout tested both probabilities
  for `ground_checks`, main `0x400` classification, nested `0x10000`
  classification, balloon context, Jango level, suit context, wall-shuffle
  `0x45` context, interaction mode, the both-timers collision decision,
  and the second collision timer. Only unlikely Jango level and likely
  both-timers beat the baseline. In particular, likely `0x400` scored
  44.765446%, likely nested `0x10000` 45.009340%, unlikely balloon
  45.339798%, and either wall context hint scored below 37.58%.
  Both interaction-mode hints regressed, with the likely hint producing
  0.0% alignment and a `0x560` frame. Those candidates were removed;
  avoid reading a single local branch as a reliable whole-function gain.
- With both successful hints retained at 46.320040%, a second sweep
  checked both probabilities for collision X/Z bounds, first/equal/second
  priority tests, the both-fixed test, distance-inside-radius, timer
  ordering, positive first timer, and the differing opponent bit. None
  improved the whole score; first-priority, positive timer, and opponent
  bit hints compiled identically. A third sweep of platform blockage,
  model `0x88`, stuck FreePlay, draw result, jump capability, outer
  FreePlay, unfixed object, teleport, and route-selected tests also found
  no improvement. Outer FreePlay and teleport hints in either direction
  scored below 44.11%, demonstrating that those guards reorder much more
  than their own branches. The two retained hints remain isolated.
- A follow-up sweep from 46.320040% tested both probabilities on twelve
  route and combat guards: route-using, route state, route frame mismatch,
  don't-toggle, route identity, combat flag, bolt defend and range, two
  defend bits, and auto-jump. None improved the whole match. The bolt
  defend bit scored 22.214080% when unlikely and 27.350216% when likely;
  it is a major block-order switch. Route frame mismatch compiled
  identically in both directions. Avoid applying broad hints to this
  region merely to recover a local branch.
- An interaction-priority sweep then tested fixed priority, three
  capability bits, path flag, movement-zero, nearby radius, opponent
  exclusion, and head-target guards. Only the unlikely head-target call
  beat 46.320040% materially. The first fixed-priority bit hinted
  unlikely scored 5.600215%, showing how sharply GCC can reorder the
  function. The likely opponent-exclusion bit gained only 0.008980
  points alone and lost 1.811061 points when combined with the retained
  head-target hint; it was removed. Hinting the final `party_under_cover`
  cleanup guard unlikely scored 42.477013%; likely scored 46.437860%,
  both below the 46.491737% unhinted form.
- On the 46.491737% layout, first- and second-player-scan hint probes
  tested both probabilities on cover context, cover FreePlay, null alert
  target, first-two-player swap, cover clearing, offscreen clearing, and
  baddie/neutral classification. None improved the whole score. The
  cover-null-alert unlikely hint compiled identically, while its likely
  form scored 43.405890%. The player-swap hints scored below 42.23%.
  Marking the complete offscreen condition unlikely scored 44.996048%
  (14,209 bytes); likely scored 46.105602% (14,197 bytes). Hinting the
  second scan's baddie and neutral category tests in either direction
  scored below 44.85%. The retained scan remains unhinted at these sites;
  local probability changes can reorder the unrolled checks and later
  blocks together.
- A follow-up measurement of all 24 permutations of the cover preheader's
  four input-register addresses found a narrow 46.252872%–46.495330%
  range at the same 14,197-byte size and `0x570` frame. The highest
  result differs from the prior 46.491737% order by only 0.003593
  points, so it should be remeasured after any substantial layout change.
- On that 46.495330% layout, isolated likely/unlikely hints for ten guards
  in the first object loop and ten in the main object-classification loop
  produced no whole-symbol gain. The first-loop probes included touch,
  capability bits, action parameter, visibility, `MiniCutCam`, and alert
  expiry; the main-loop probes included `0x400` category, interactive
  classification, opponent state, pad null, and the AI-processing guard.
  Keep these conditions unhinted until a structural change changes the
  register and block layout.
- The antinode timer clamp's original instruction sequence is `movaps`
  of the decremented timer, `cmpnltss` against zero, then `andps` with the
  decremented timer. `cmpnltss` makes an all-one mask on an unordered
  comparison, so the target retains the timer in that case. The earlier
  `!(timer >= 0.0f)` source instead clears it. The corrected `timer < 0.0f`
  condition emits `maxss` and scores 46.447918% (14,197 bytes, `0x570`
  frame) against the former 46.495330%. Reversed comparison and ternary
  spellings compiled identically. An `_mm_cmpnlt_ss`/`_mm_and_ps` version
  emitted the target mnemonics with extra moves and scored 46.219826%; an
  exact three-instruction inline sequence scored 46.411636% when volatile
  and 46.426006% otherwise. All altered register allocation or scheduling
  around the loop. The corrected plain condition is retained for its
  unordered behavior; a local opcode match did not improve the full diff.
- In the following avoidance loop, the target reloads the interactive
  array base from `esp+0x114`; the current build derives it from the
  neutral-array base plus `0x100`. Making `interactive_objects` an
  addressable read/write asm operand immediately before the mode branch
  caused a separate base reload, but grew the frame to `0x580` and scored
  43.800290% at 14,135 bytes. Shrinking the padding array restored the
  `0x570` frame yet scored only 43.800648%. Both changes were reverted.
  Match array-base allocation and its lifetime together; forcing one
  stack reload can disturb the entire later register schedule.
- Retesting the post-`AISysProcessCharacter` non-player branch on the
  46.447918% layout confirms that its target-like fallthrough direction is
  still globally worse: marking it unlikely scores 36.011135% with a
  `0x560` frame, while removing its likely hint scores 41.213722%, also
  with `0x560`. Its retained likely hint keeps the `0x570` frame.
- On the same baseline, isolated likely/unlikely probes in avoidance for
  interaction mode, X and Z bounds, the both-fixed test, inside-radius,
  and opposite-team collision did not beat 46.447918%. The opposite-team
  hint compiled identically in either direction. The inside-radius
  unlikely hint and both-fixed likely hint each scored 46.278020%.
  Forcing the pointer into `edi` with an input constraint scored
  40.264008% and shrank the frame to `0x560`; an unconstrained input is
  more stable here than an apparently target-like register constraint.
- The retained two-input avoidance marker is position-sensitive. Moving
  the pointer-only input before the LOS call scored 46.243176%, after the
  LOS call 46.291668%, before the avoidance timing call 44.572197%,
  inside the new-interaction branch 46.371048%, before the pair loop
  46.053880%, and before the antinode call 46.449710%. With the marker
  at its retained pre-branch position, adding the interaction count,
  `WORLD`, or frame-time address to the pointer-plus-mode-address inputs
  also regressed. Under the retained pointer-plus-mode-value layout, an
  inline `cmpnltss`/`andps` timer clamp scored 46.362790% as volatile
  asm and 46.402298% as nonvolatile asm. These probes were reverted.
- The current main-loop object pointer stays in `esi`, while the target
  uses `edi`. Disassembly shows a likely cause: the current player scan
  forms the neutral-array base in `edi` at `esp+0x170`, carries it through
  the main AI loop, and uses it in the neutral-to-baddie append. The target
  saves that base at `esp+0xdc` and later addresses the source array from
  the stack, leaving `edi` for the object pointer. Directly constraining
  the object to `edi` at five loop locations scored 38.067886%–39.289154%
  and shrank the frame to `0x560`. Clobbering `edi` at four scan-to-loop
  boundaries scored 39.511852%–39.549210% with the same smaller frame.
  An input-memory constraint on `neutral_objects` before the main loop
  kept the `0x570` frame but scored 39.974857%. These interventions did
  not reproduce the target's register handoff and were removed.
- Replacing the first player scan's neutral cursor with indexed stores
  scored 39.877872% and shrank the frame to `0x550`; increasing padding
  to restore `0x570` scored 39.880028%. Indexing the two main-loop goody
  stores instead scored 45.267960% with a `0x560` frame. The original
  cursor forms were restored. The long-lived neutral base is a real
  mismatch, but altering one collection cursor changes more than that
  register's lifetime.
- Loading `route_suit_index` into a local before `Suit_GetIndex` in the
  route-toggle tail scored 46.115660% despite seeming closer to the
  target's load order. Requiring that local in a register before the call
  scored 46.466236%, just below the retained 46.468033%. Both forms were
  removed; a local scheduling match still needs the surrounding register
  and stack layout.
- Remeasuring all 24 cover-preheader address-input orders on the
  46.468033% layout gave 46.225574%–46.468033%. Four orders tie for the
  maximum, including the retained one; the earlier optimum remains valid.
  The output-only stack-padding array scores 46.468033% at lengths 12,
  16, and 20 bytes, all with the target `0x570` frame. Lengths 1–8 and
  24–64 score 46.464798% and shift the frame; 16 bytes remains retained.
- A call-site inventory of the retained binaries finds 69 direct calls on
  each side with the same helper multiplicities after normalizing the
  `TestWalkAround` IPA-clone suffix. Only 18 calls occupy the same ordinal
  position; their address-order sequences have a 54-call longest common
  subsequence. After `AISysProcessCharacter`, the target places
  `BoltType_FindByID`, `SetObjAsHeadTarget`, and the old interaction helper
  before its wall-ray helper calls; the current build places wall-ray calls
  first and those combat calls late. This points to block placement and
  register allocation rather than a missing call site. The clone suffix
  differs (`isra.10` versus `isra.19`), so compare its machine body
  separately.
- On the 46.468033% layout, `__restrict` on the neutral and baddie base
  pointers compiled identically and left the neutral-to-baddie overlap
  guard. Direct `object_lists[3]`/`object_lists[0]` indexing in that copy
  removed the guard but scored 28.987068% (13,770 bytes). Replacing the
  loop with `memcpy` scored 41.236350% (13,813 bytes, `0x560` frame) and
  did not move the main object into the target's `edi`. These alternatives
  were removed; eliminating the local guard alone disrupts the wider CFG.
- An explicit `goto interaction` for the final head-target guard compiled
  identically at 46.468033% when the null path was marked likely.
  Removing the hint scored 46.296337%; marking null unlikely scored
  45.948635%. The retained source already gives GCC the best measured
  block placement for that guard.
- Using `MiniCutCam` directly in the first loop moves its GOT load to the
  target's later preheader position but omits the target's stack store and
  scores 46.409480% (14,189 bytes). Loading and storing an addressable
  cache only after a positive `HIGHGAMEOBJECT` guard puts it before the
  touch/draw-distance setup and scores 43.766163% (14,201 bytes). The
  original preloop cache remains retained. In the Jango float comparison,
  the target's `jnp` branches to a distant `je`; together those handle
  ordered equality and unordered comparison. Do not interpret the `jnp`
  alone as a changed NaN predicate.
- Forming a fresh neutral-source pointer immediately before the LOS append
  and passing it through an opaque read/write or tied-output register
  operand scored 39.830460% (14,039 bytes, `0x560` frame), whether it was
  initialized from `neutral_objects` or `object_lists[0]`. An input-only
  register use compiled identically to 46.468033%. The opaque handoff
  shortens the apparent pointer lifetime but also rewrites the main loop's
  register and stack allocation; it was removed.
- The first cover scan's positive accumulator still emits eight `cmove`
  instructions on the 46.468033% layout; the target has seven `cmovne`
  and one `cmove`. An equivalent inverted accumulator scored 43.747845%
  and still emitted eight `cmove`. A read/write memory constraint after
  each positive update emitted eight `cmovne`, but scored 41.018320%
  with a `0x580` frame, or 41.021910% after restoring `0x570` padding.
  Matching the conditional-move opcode alone is not enough; the current
  scan also reloads the `Player` GOT pointer each unrolled iteration.
- A plain local alias for the first scan's `Player` base compiled
  identically. An input-register use scored 46.147270%, pinning it to
  `esi` scored 44.615303%, and a read/write register operand scored
  45.547770%. A pointer cursor scored 45.509697%. Carrying the next
  player as a separate value moved its load before the follow-offset
  store but scored 44.714440%; moving the load to just before that store
  gave a locally closer sequence yet scored 14.678520% because GCC
  reordered much more of the function. All forms were restored.
- Individually hinting the ten `TimingBarSet == 4` guards around Sys,
  LOS, Avoid, Opponent, and Turret found no whole-symbol gain. Marking
  LOS-open, Avoid-open, or Turret-open likely compiled identically at
  46.468033%; the other 17 probes scored lower, from 43.672413% to
  45.912716%. Swapping the `baddy_count + neutral_count` expression or
  spelling it as a two-step increment also compiled identically. GCC
  canonicalizes those LOS count forms on this layout.
- Immediately after that call, the target emits `jns` to the distant
  non-player wall path, leaving the player/route/combat path in fallthrough.
  The retained build emits `js` to the later route/combat path and falls
  into the wall path. An explicit conditional jump plus labels, with the
  non-player branch marked unlikely, recovered `jns` but scored only
  35.834053%. Adding a likely hint on the route guard improved that probe
  to 40.559986%, still below baseline. The one branch direction is not
  the sole cause of the surrounding block and register mismatch.
- Hints on the full post-AI `FreePlay && field_0x1f4` route guard scored
  42.458332% likely and 43.819324% unlikely. Hinting only `FreePlay`
  scored 40.631466% likely and 44.761852% unlikely. Hints on the combat
  `field_0x1f8` guard scored 17.737429% likely and 43.107758% unlikely.
  Hints on the entire bolt-defend guard or its opponent-pointer term also
  lowered the score (best 45.336926%). The target's call order is not
  recovered by independently hinting these guards.
- Reordering the source blocks with labels while preserving the original
  execution order moved combat before the non-player and route source
  blocks and scored 46.471622%, only 0.003589 points above the retained
  46.468033% baseline at the same 14,205-byte size. Reordering combat,
  route, and wall blocks to follow their target call order scored
  45.228810%; two other permutations scored 45.228810% and 46.037000%.
  A redundant `goto` at either existing label compiled identically to
  baseline. This large source rearrangement was initially deferred for
  such a small whole-symbol gain; it was retained later with the baddy
  cursor handoff. Source order alone does not determine GCC's emitted
  block order.
- The target's main object pointer uses `edi` where the current loop
  tends to use `esi`. A scoped `register` variable bound to `edi` scored
  42.614223% and still used `esi` in the hot loop. Empty `+D` constraints
  before or inside the loop scored 39.235992% and 39.341236%; an input
  `D` constraint scored 39.195040%. Forcing one register at the source
  level changes other live ranges and is not a local fix.
- The antinode timer loop uses `cmpnltss` plus `andps` in the target,
  while the retained source emits `maxss`. Spelling the target operation
  with `_mm_cmpnlt_ss` and `_mm_and_ps` emitted those opcodes, but chose
  different XMM registers and reduced the whole-symbol match to
  46.333332% (14,221 bytes). `_mm_set_ss` and `_mm_load_ss` forms compiled
  identically. Matching two local opcodes did not compensate for the
  changed register allocation and surrounding loop layout.
- In that loop, the target indexes directly from its `edi` list pointer;
  the current build uses an extra `0x100` displacement. Pinning
  `interactive_objects` to `edi` with input or read/write constraints at
  the pre-loop marker or inside the loop scored 40.035560% to 44.839798%.
  Swapping the temporary list assignments so interaction starts at
  `object_lists[0]` scored 45.133263%; other tested list permutations
  scored 42.650143% to 46.465157%. The local address-form mismatch
  cannot be fixed independently of the four lists' live ranges.
- Renaming the main-loop pointer to a new local, shadowing the first-loop
  pointer in a nested scope, and wrapping the existing pointer in a scope
  all compiled identically at 46.468033%. GCC's loop allocation depends
  on live values rather than those source-level names or scopes.
- Saving the neutral-list base before the main loop and reading it for the
  later neutral-to-baddie append did not free `edi` favorably. Input-only
  and read/write memory handoffs both scored 40.512930%; a volatile
  pointer scored 42.416668%, or 42.436060% with a read/write memory
  constraint. The current target-like stack handoff needs a different
  data-flow shape, not merely a forced local spill.
- Moving combat, wall, and route source blocks into the target's broad
  address order with an explicit non-player dispatch scored 41.167385%
  unhinted, 46.471622% when non-player was likely, and 35.959410% when
  non-player was unlikely. The likely case produces the same tiny gain as
  the earlier combat-before-precombat move; GCC mostly reconstructs the
  retained layout. The target's `jns` branch is recovered by the unlikely
  case, but the wider function then diverges more.
- Retesting the retained post-AI non-player guard directly on the current
  46.468033% layout gives 41.165590% without its likely hint and
  35.963000% with an unlikely hint. The earlier positive result for the
  likely hint still holds after the subsequent source changes. Ghidra's
  target decompilation gives the same logical wall, route, combat order as
  the current source; the remaining difference is emitted block placement.
- On the 46.520473% layout, replacing both main-loop baddy cursor stores
  with indexed writes scored 45.333690%; replacing both goody stores
  scored 44.712643%, and both interactive stores scored 45.815372%.
  Switching the main-loop neutral indexed write back to its player-scan
  cursor scored 45.565730%. These are behavior-preserving only when both
  sites for a given cursor change together; one-site mixtures can overwrite
  prior entries. The retained pointer-store choices still matter after the
  new baddy handoff.
- Stack placeholder lengths 12, 16, and 20 bytes still tie at
  46.520473% with a `0x570` frame. Lengths 1–8 and 24–64 score
  46.517242% and shift the frame. At the baddy handoff, `+g` and `+rm`
  compile identically to `+r`; fixed `eax`, `ecx`, or `edx` operands score
  46.407690%, 46.405890%, and 45.815014%. Reversing the five retained
  cover, Jango, head-target, wall-scale, and antinode-timer branch hints
  also lowers the whole-symbol score. The current result depends on the
  combined register and branch layout rather than a single local match.
- After removing the `MiniCutCam` memory operand, ablating the other 15
  standalone empty asm markers one by one did not exceed 46.533405%.
  The closest was removing the first-loop object input (46.531610%).
  Dropping the baddy handoff tied at 46.533405% and kept the same 14,189
  bytes, while changing 25 already-mismatched stack operands; it was
  removed. The other markers still help this combined layout. Removing
  the combat source reordering scores 46.529810%, and moving the `Obj`
  assignment back after the cursor declarations scores 46.450790%; those two
  layout choices are still worth retaining.
- Repeating the ablation on the later 46.595905% first-loop layout found
  no beneficial standalone-marker removal. The closest is dropping the
  first-loop object input (46.594110%); removing the new bound or camera
  input scores 46.577946% or 46.572918%. All 24 cover-preheader GOT-input
  orders still tie or lose to the retained order. The combat source move
  still contributes 0.003595 points, and placing `object = Obj` after the
  two cursor declarations scores 46.543823%. Placeholder lengths 12, 16,
  and 20 bytes tie at 46.595905% with the target `0x570` frame.
- Before `AISysProcess`, GCC loads the `WORLD` GOT address after storing
  `ai_fighting`, while the target loads it before that store. Staging a
  cached world pointer scores 46.505750%; empty input markers on `&WORLD`
  or `WORLD` score 46.477013% and 46.509340%. Staging the three player/world
  GOT addresses, their values, or three cached values scores 46.520115%,
  46.548492%, or 46.584770%. The closest three-cache variant still loses
  to 46.595905%; moving these loads independently disturbs later register
  allocation.
- A single empty marker binding `&ai_fighting` to `eax`, `&player2` to
  `edi`, `&player` to `edx`, and `&WORLD` to `esi` exactly recovers the
  target's four-GOT-load order and registers at the first call site.
  The whole function falls to 45.126440% and grows to 14,189 bytes.
  Three- and two-address fixed-register versions score 45.112070%,
  while early value loads score 44.123560% or 43.632540%. Exact prologue
  register choices are therefore a poor proxy for the later live ranges;
  none of these markers was retained.
- The first-loop capability word is copied from `ecx` to `esi` in both
  builds, but the current build spills the original `ecx` to `[esp+0xe0]`
  before testing bit 0; the target uses `and ecx, 1`. An explicit local
  for the original word compiles identically. An input-only `"c"` marker
  on that local emits the target-like `and ecx, 1`, but scores 46.308548%
  when only bit 0 uses the local and 46.247128% when all three bits do.
  A general-register marker scores 46.574352%. These local opcode gains
  shift the high-flag stack slot and later layout, so the simpler word
  update remains retained. Moving the `"c"` marker after the masked-word
  store scores 46.233116%; making it read/write before or after that store
  scores 46.308190% and 46.233116%. Operand timing does not resolve the
  wider lifetime change.
- A scratch array kept live across the first object loop with empty
  memory output/input markers does not move the early local slots into
  the target positions. Eight bytes compile identically at 46.595905%;
  sizes 16 through 64 grow the frame from `0x570` to `0x580`–`0x5b0`
  and score 46.592674%. For this GCC, adding an artificial live region
  here grows the frame instead of shifting the useful slots within it.
  Shrinking the existing post-scan placeholder while adding a 16-, 32-,
  or 48-byte early array did not create a new layout: the best combinations
  either compile identically at 46.595905% (`16` early, `1` or `8` late)
  or keep the frame growth and score 46.592674%.
- Declaring the cached `MiniCutCam` value `volatile` frees `edi` for the
  first-loop bound and avoids its `[esp+0xe8]` spill, but scores 46.579742%
  at 14,189 bytes. Keeping the `"c"` input scores 46.480960%; input or
  read/write memory markers on a nonvolatile cache score 46.452587% and
  46.540590%. Ablating each remaining standalone marker from the volatile
  variant did not reach the retained 46.595905%; the best was removing
  `party_under_cover`'s memory marker at 46.583332%. Forcing the bound to
  `edi` with that volatile cache scores 45.705820%, and `ecx` scores
  46.475574%. Removing the bound marker, adding memory markers to the
  volatile cache, or combining removal of the party marker with a one-byte
  frame placeholder also scores lower. Getting the target bound register
  alone is insufficient; the extra camera stack home and wider allocation
  still diverge. Pinning only the `HIGHGAMEOBJECT` GOT address to `edx` in
  this volatile layout scores 46.608480% and is retained; pinning `Obj`
  too scores only 45.860634%. Removing the formerly helpful
  `party_under_cover` memory input then raises the score to 46.612070%,
  and reordering the four cover GOT-address inputs raises it to 46.613865%.
- On the 46.613865% layout, another 14-marker ablation found no gain:
  removing the first-loop object or bound input ties, while all other
  removals lower the score. Varying the output-only frame placeholder to
  12 or 20 bytes ties at `0x570`; 1–8 bytes give 46.610634% and a
  `0x560` frame, while 24–64 bytes grow the frame and give the same lower
  score. Reversing the five retained head-target, Jango-level, final-cover,
  both-timers, and wall-scale hints all regresses. Removing or reversing
  the first-loop eligibility, touch-sign, or alert-expiry hints also
  regresses. These choices remain stable after the camera/cache change.
- Wrapping the first object loop in a positive `object_limit` guard does
  move the `MiniCutCam` GOT load after the target-like positive-bound test.
  An unhinted guard scores 44.401940%, and a likely guard scores
  45.622845%; making the guarded loop's latch use `!=` scores 44.352370%
  or 45.573277% with the likely guard. The local load schedule matches
  better, but the block layout changes much more and was restored.
- Removing the memory operand while leaving the named `high_flags` local
  compiled identically to the 39.445040% index-only build (14,061 bytes).
  GCC folds the local unless its addressability is part of the source
  constraints; the memory input was restored.
- Nesting the first object loop's action-frame logic around
  `use_action_frames == 0`, with the decrement in `else`, lowered the current
  linked-GOT score from 26.952227% to 26.654095% (size 13,485 to 13,469).
  This source spelling did not recover the original's long-lived zero in
  `xmm1`; the earlier `if (frames != 0) ... else if ...` form was restored.
- Ghidra represents the initial party-cover scan with a local `local_464`
  decision and a final store to `party_under_cover`; the current source writes
  that global during the scan. An explicit `i32 cover_result` with a final
  global store is behavior-equivalent here because the scan makes no calls,
  but it lowered the linked-GOT score from 26.952227% to 25.312500% (size
  13,485 to 13,322) and reduced locally identical instructions in the scan
  from 12 to 10. GCC did not preserve the decompiler's apparent stack form;
  the source trial was reverted.
- Splitting the two wall-ray distance decision into nested first-distance and
  second-distance branches lowered 26.834412% to 26.179598%. The target's
  decompiled branches are more detailed than the existing source expression,
  but reproducing that nested spelling did not recover their machine layout.
- The target's first-hit/second-hit ray decision uses `ucomiss` of the
  `1.0e9f` limit against the first distance, then `ja` to select a direction
  immediately; otherwise it checks the second distance. A NaN first distance
  does **not** take `ja`, so the second distance decides. Ghidra's decompiled
  `if (limit <= first_distance)` form suggested a different unordered path.
  Changing source to `!(limit <= first_distance) || second_distance < limit`
  lowered the later 37.488148% baseline to 37.456540% and introduced a
  different NaN path. The original `first_distance < limit ||
  second_distance < limit` was restored. Inspect condition-code branches, not
  just decompiled float comparisons, before treating a NaN case as a bug.
- The original keeps both the pre-mask and post-mask AI capability words live.
  Expressing those as two explicit C++ locals emitted essentially the same
  first-loop machine code and measured 26.658047%. Source-level data-flow
  names alone did not change the register choices. Repeating the explicit
  pre-mask and masked-word locals after the later loop and array changes
  compiled identically at 37.798850% and 14,051 bytes; the simpler source
  was restored.
- In the route-toggle initializer, replacing the conditional expression for
  `route_suit_index` with explicit null and non-null branches looked closer
  to the target's separate `-1` store. The GOT-aware whole-symbol score
  nevertheless dropped from 38.002514% (14,117 bytes) to 37.490303%
  (14,132 bytes). The conditional expression was restored. This is another
  case where a local branch resemblance caused worse global block alignment.
- Unlike the stuck-toggle timer, spelling the later route-toggle timer's
  zero/quarter-second choice as explicit branches compiled identically to
  its ternary form: 38.002514%, 14,117 bytes. GCC's result is context
  dependent even within the same function; the shorter form was retained.
- In the interaction bounds checks, Ghidra renders ordered `<=` tests. The
  original assembly uses `ucomiss` followed by `ja` for the upper-bound reject,
  so an unordered/NaN comparison does **not** take that reject branch. Writing
  the decompiler's `<=` expressions directly changed the branches to `jb`
  and measured 26.830818% versus 26.834412%. The original `>`/`<` reject
  expressions were retained. Verify floating-point branch flags in assembly
  before treating decompiled comparisons as NaN-exact source.
- At 46.617817%, the linked-GOT aligned call map shows the same major call
  sites but very different physical block order. The target's wall-ray
  helpers appear around aligned indices 1975–2045; the current build puts
  several at 1021–1105. The target's `BoltType_FindByID` call is at 936
  versus 2386 in the current build, and `LEGO_AISysCreatureInteraction2D`
  is at 1072 versus 1576. This is a block-placement mismatch in addition
  to register and stack differences, not evidence that those calls are
  absent. Putting route source before wall source while preserving wall →
  route → combat execution scores 45.373203% (14,261 bytes), or 45.441093%
  with an empty marker at the wall-to-route jump. The earlier combat-first
  source order remains better.
- An explicit post-`AISysProcessCharacter` non-player dispatch, with wall
  and route labels, scores 46.617817% when the non-player branch is hinted
  likely: GCC reconstructs the retained code. Removing the hint scores
  41.312860%, and marking non-player unlikely scores 36.106680%. This
  confirms that changing only the source shape of that dispatch does not
  move the cold wall-ray block into the target's address range.
- Retesting all 24 cover-preheader address-input orders after the first-loop
  `Obj` pointer was pinned to `ecx` still selects the retained order at
  46.617817%. On this layout, forcing the first-loop bound to `ecx`, `eax`,
  `edi`, `esi`, or memory scores 44.785560%–46.418823%; read/write `+r`
  ties the retained input-only `"r"`. For the `high_flags` byte, input
  `"r"` ties the retained `"c"`, while fixed `eax`/`edx`/`esi`/`edi`,
  memory, and read/write register forms regress. Repeating standalone
  marker ablation found no gain. These constraints remain locally stable.
- An explicit positive `object_limit` guard with an `index != object_limit`
  latch emits a `jne` in the first loop, closer to the target's branch,
  but GCC compares `edi, edx` rather than target `edx, edi` and reorders
  more blocks. The unhinted form scores 44.288433%, likely 45.509340%,
  and unlikely 39.938217%. Keep the `<` latch until its surrounding
  data flow can also be matched.
- Keeping the cached `MiniCutCam` volatile but typing it as `u32` or
  `const i32` compiles with the same 46.617817% whole-symbol score.
  Normalizing it to a volatile zero/nonzero `i32` scores 46.461210%; a
  volatile `bool` scores 46.298134% and adds 16 bytes. The integer value's
  stack lifetime, rather than signedness or const qualification, is the
  useful part of the retained source.
- At the post-`AISysProcessCharacter` dispatch, an `asm goto` with a
  sign-bit compare emits the target's local `cmp`/`jns` and leaves the
  player-side FreePlay tests in fallthrough. The whole function scores
  only 37.879310% (14,155 bytes) with an unconstrained object operand;
  forcing that operand to `edi` scores 33.524784% (13,992 bytes). Marking
  the wall label cold and combat/interaction labels hot, separately or
  together, compiles identically at 46.617817%. Exact local branch bytes
  or label attributes do not reproduce the target's later block order.
- The target's first player scan saves its neutral-list base to a stack
  slot and frees `edi` for the main object loop. The current build keeps
  that base in `edi` from `lea edi, [esp+0x170]` through the main loop,
  so the object advances in `esi`. Advancing `neutral_cursor` at the main
  neutral append and deriving the list start from the final cursor is
  behavior-equivalent, but scores 39.743893%–40.344467% and still uses
  `esi` for the object. Capturing the neutral base after the player scan
  in a volatile pointer or integer scores 42.970547% (13,789 bytes);
  copying that saved pointer back to a nonvolatile local or using a
  read/write memory marker scores 40.350216% or 40.166310%. A separate
  stack handoff alone is therefore insufficient; the scan, main-loop
  collection, and later neutral copy must be matched together.
- On the 46.617817% layout, retyping the avoidance timer clamp with SSE
  intrinsics emits target-like `cmpnltss` and `andps` but scores
  46.483116% (14,205 bytes). An equivalent two-op inline-asm mask scores
  46.449710% nonvolatile or 46.530533% volatile. Neither fixes its
  surrounding XMM allocation. Rechecking the frame placeholder gives the
  same score for 12, 16, or 20 bytes with a `0x570` frame; 1–8 or 24–64
  bytes score 46.614582% and change the frame size.
- A local Git history search for the `GameAIProcess` declaration finds its
  introduction but no later wholesale replacement of that named entry.
  Comparing the extracted function body across all local refs found the
  current 35,638-byte body identical on `main` and the active matching
  branch; older refs carry only 1,147- or 1,178-byte stubs. There is no
  hidden higher-match implementation in those refs. The reference binary
  has symbols and a GCC 4.7 `.comment`, but no debug sections with source
  lines. Use disassembly and the linked-GOT fork as the matching evidence.
- Combining a post-scan volatile neutral-base save with a read/write
  `edi` constraint on the main-loop object still does not recover the
  wider layout. A pre-loop object constraint scores 39.017600%; placing
  it inside the loop does make the object advance in `edi`, but scores
  39.887573% and shrinks the function to 13,879 bytes. The target register
  alone is not enough; the saved base, list cursors, and surrounding
  branches all change. Keeping an unrelated pointer live across the main
  loop through paired empty register markers scores 44.511852%–44.608116%
  for six tested values and still advances the object in `esi`. General
  register pressure does not selectively spill the neutral base.
- Explicitly caching `GAMEPAD_START` before the first object loop scores
  46.418460% with a plain or input-register local, 46.263650% with an
  input-memory marker, and 46.234913% with a volatile local. Declaring
  the cache at its two-button use site compiles identically to direct
  global references at 46.617817%. The target's preloop mask store must
  be approached through the surrounding stack allocation, not a simple
  source cache. Reassigning the four list-buffer roles also regresses:
  three tested orders score 42.304596%–45.399784%, and the closest
  neutral-first order scores 46.614940%. Retain the role order that matches
  the target's four array bases.
- On the retained first-loop touch-capability cascade, marking each of the
  three bit tests individually likely compiles identically at 46.617817%.
  Marking bit 0, bit 1, or bit 5 unlikely scores 44.242460%, 44.243176%,
  or 44.521553%, and shrinks the body to 14,186 bytes. Although the
  target sends the bit-5 set case to a distant block, forcing that one
  branch cold reorders more than the desired local block. The retained
  unhinted cascade preserves the best whole-function layout.
- The first player scan swaps the first two players when `Player[1]` is the
  active player. Writing that as a direct `Player[1 - index]` selection makes
  GCC negate the index and retain extra loads. Assigning `Player[1]` in the
  guard, then selecting `Player[0]` only when `index != 0`, improves the
  linked-GOT score from 46.617817% to 46.712643% and shrinks the function
  from 14,189 to 14,101 bytes. A ternary selection scores 46.454742%; a
  separately assigned branch scores 46.537357%. The assignment in the
  guard is part of the useful GCC control-flow shape.
- With that direct swap guard, making the first-scan `neutral_cursor` local
  volatile improves 46.712643% to 46.764730% at the same 14,101-byte size;
  an addressable read/write memory operand scores 46.727013%. Caching the
  `Player` base for both indexed reads lowers the score to 43.960130%,
  whereas caching any one of the four reads or the first three compiles
  identically to the no-cache 46.712643% layout. Applying volatile to the
  goody, interactive, or baddy cursors after the neutral change scores
  46.276580%, 46.381824%, or 46.714798%; pairs and all three also regress.
  Keep only the neutral cursor volatile on this layout.
- Continuing the volatile neutral cursor through the main object loop's
  neutral append ties at 46.764730% and 14,101 bytes. It changes the local
  append from an indexed stack-array store to a cursor store, as in the
  target, without a whole-symbol penalty. Separating the pointer increment
  from the store scores 46.514008% and grows the function to 14,117 bytes.
  Later copying from `object_lists[0][index]` directly rather than through
  `neutral_objects[index]` scores 35.934270%; directly indexing the baddy
  destination scores 33.733116%, and doing both scores 34.215520%.
  Stack-array syntax alone makes GCC rewrite the long-lived pointer and
  overlap handling, so retain the aliases for the copy.
- Per-function GCC options are not a substitute for the target's source
  control flow: `no-reorder-blocks` and `no-guess-branch-probability`
  collapse the whole-symbol match to 0%, `no-tree-partial-pre` scores
  44.040947%, `no-tree-vectorize` 39.536278%, and `O2` 34.536636%.
  `no-reorder-blocks-and-partition` and `no-ipa-cp` compile identically to
  the 46.617817% baseline. These probes kept the build's target toolchain
  and changed only a function `optimize` attribute.
- On the 46.764730% layout, the target's first party-cover scan appears to
  keep a local decision and store it to `party_under_cover` after all eight
  checks. Replacing the per-player global writes with an `i32 cover_result`
  preserves behavior because the scan makes no calls. A plain local scores
  45.640804%, and a volatile local scores 46.658405%; an initial empty
  read/write memory operand for the local raises the score to 46.892960%
  and gives it a stack home. Moving that operand to the final store or an
  update site scores 44.116380%–44.974136%. An input-only memory operand
  scores 46.834410%, and an input register scores 45.590878%. The lifetime
  of the addressable local, not just its final store, shapes this scan.
- With that local cover decision, initializing `party_under_cover`, then
  `active_neutral_count`, then the local and its memory operand scores
  46.924927% (14,205 bytes). Putting the local between the two globals
  scores 46.892960%; all 24 permutations of those three initializations
  and `all_under_cover` tie or lose to the retained order. Rechecking all
  24 cover GOT-address input orders, the combat source order, the main
  object assignment order, and each standalone empty asm marker found no
  further gain on this layout.
- Deferring the neutral-base declaration until the later list copy compiles
  identically to the 46.764730% baseline: GCC still keeps the stack-array
  address live across the main loop. A read/write register handoff for the
  deferred pointer scores 31.209412%. Replacing the two-dimensional list
  storage with four separate arrays scores 42.483116%; a struct of four
  arrays scores 31.841595%. The original two-dimensional storage remains
  necessary for the closest stack offsets and vectorized list copy.
- On the 46.924927% cover-local layout, the frame placeholder can be 4,
  8, 12, 16, or 20 bytes without changing the score or `0x570` frame;
  one byte shrinks the frame and 24 bytes or more grows it, lowering the
  score to 46.921696%. Extra output-only scratch homes can shift the
  cover-result stack slot from `esp+0xf8` to `esp+0x104` or `esp+0x108`
  while balancing the frame placeholder, but the whole score ties. Five
  such scalar homes grow the frame and score 46.921696%. Artificial
  scratch space alone does not recover the target's `esp+0x11c` result
  slot or its `eax` reload.
- Moving the cover-result initialization inside the first player iteration
  scores 41.787716%–41.803520% with the memory operand and
  45.530890%–45.640804% without it. Forcing an `eax` or general-register
  handoff just before the final global store scores 44.848778%–45.532690%;
  a volatile-only final reload scores 46.284843%, an atomic relaxed load
  44.877872%, and an explicit `eax` load 44.961926%. The target-like
  local load cannot be isolated from the wider scan register allocation.
- On this layout, expressing the positive `all_under_cover` update as an
  explicit zeroing branch scores 46.883980%; a separate select-keep local
  compiles at the retained 46.924927%, while bitwise/logical conjunction,
  byte-sized accumulators, and volatile accumulators regress. Twelve
  individual hints on the cover context, disabled flag, special context,
  FreePlay, model-cover flag, and null alert target tie or regress. Keep
  the existing 32-bit ternary and its branch probabilities.
- Retesting the second player scan's Ghidra-style local cover value after
  the first scan gained its local still scores below the retained layout.
  A plain local scores 46.820040%, a volatile local 46.721622%, an
  input-memory marker 46.469467%, a read/write memory marker 46.655890%,
  and an input-register marker 46.757904%. There are no calls within that
  scan, so the rewrite is behavior-equivalent, but its stack and register
  handoff does not reproduce the target's. Keep the direct global update.
- The first-loop cached `MiniCutCam` local can be `volatile u32` or
  `const volatile i32` with identical 46.924927% codegen. Normalizing it
  to a volatile integer boolean scores 46.791668%, and a volatile `bool`
  scores 46.748924%. The raw camera integer's stack lifetime is still
  the useful part of that cache.
- The target loads the `Player` GOT address once, while the 46.924927%
  build loaded it 20 times across the two scans. A plain first-scan alias
  compiles identically, but handing that alias through an empty read/write
  general-register operand reduces the current load count to 13 and raises
  the whole-symbol score to 47.559628% (14,221 bytes). Pinning it to `esi`
  scores 46.204742%; a volatile pointer scores 46.995690%. The opaque
  pointer value helps GCC retain the array base without changing the
  player elements read on each iteration.
- Reusing that first-scan pointer for the second scan's `Player[0]` swap
  and indexed fallback reduces current `Player` GOT loads to 11 and raises
  the score to 48.006824% (14,221 bytes). Reusing it for every second-scan
  read lowers the score to 44.482758%; giving the second scan its own
  pointer also regresses. Of all 15 nonempty subsets of its four reads,
  the retained two-read subset and one alternative both score 48.006824%.
  Extra handoffs before or after `AITriggerSetSysProcess` or the second
  scan do not improve it. Pointer reuse must be measured per read site.
- On the 48.006824% layout, the formerly useful input marker on
  `ai_update_flags` now lowers the score; removing it raises matching to
  48.290947% and shrinks the function to 14,165 bytes. A four-marker
  combination sweep then found that removing the route-toggle hold-time
  memory marker and the first-loop `Obj`/`HIGHGAMEOBJECT` GOT constraint
  raises matching to 48.506107% (14,157 bytes). Removing either alone
  scores 48.504670% or 48.290590% on the intermediate layout, so their
  effects are not independent. A fresh ablation of all remaining empty
  markers on 48.506107% found no further gain. Retest old constraints
  after source data flow changes instead of assuming they still help.
- Eight of the 48.506107% build's eleven `Player` GOT loads came from the
  unrolled cover cleanup loop: it read `Player[index]` separately for its
  null test and field update. Reusing the early `player_slots` pointer
  there removes those loads but extends its lifetime across too much of
  the function and scores only 46.747486%. Introducing a fresh
  `cleanup_players = Player` alias inside the cleanup guard, handing it
  through an empty read/write general-register operand, and loading one
  object per iteration raises the score to 49.312500% (14,109 bytes) and
  leaves four current `Player` GOT loads versus the target's one. A plain
  alias compiles identically to the prior layout; fixed `esi`/`edi`,
  input-only, and volatile variants score 47.278378%–48.423492%.
  Pointer lifetime matters as much as the number of GOT loads.
- With that cleanup alias, all 16 second-scan read-site subsets were
  remeasured. The retained pair, `Player[0]` in the swap and the indexed
  fallback, still wins at 49.312500%; caching all four reads scores
  44.475574%. A separate opaque pointer for the remaining condition and
  follow-offset reads scores at most 47.545258% (volatile form). Removing
  the old `interactive_objects`/`test_new_interaction` input marker then
  raises the score to 49.355960% (14,101 bytes). A fresh ablation of the
  remaining markers and a full cover-address order sweep found no further
  gain. The frame placeholder still ties at 4–20 bytes with the target
  `0x570` frame.
- On the 49.355960% layout, removing or reversing each of the 14 retained
  branch hints found no gain; the closest unhinted path-connection test
  scores 49.253593%. Keeping the cleanup pointer local but moving its
  declaration and read/write register handoff just before the cover guard
  raises the score to 49.369614% at the same 14,101-byte size. Moving it
  before the `(Avoid)` close call scores 48.959410%; hinting the guard
  likely ties, while unlikely scores 45.054237%. The call boundary is a
  useful limit for this pointer's register lifetime.
- Saving the first `player_slots` pointer to a volatile or addressable
  stack local for cleanup reduces current `Player` GOT loads from four to
  three, but scores at most 49.198635%. Saving the neutral-list base
  similarly can move the main object into target register `edi`, yet the
  best whole-symbol score falls to 33.377155% and the array-copy layout
  changes. Exact GOT-load count and a local object-register match do not
  outweigh the surrounding live ranges. Removing the combat source
  reordering after the cleanup-pointer gain scores 49.366020%, just below
  the retained ordering.
- On the 49.369614% build, forcing an input-only memory use of the
  `neutral_objects` base at its initialization, just before the main
  loop's declarations, or immediately before that loop scores
  31.349138%, 31.375000%, and 31.377155%, respectively. Even a read-only
  addressable use changes the list base's register and stack allocation
  across the function; it is not a narrow way to free `edi`. A read/write
  register handoff of that base at the same three points also scores only
  31.363146%–31.561064%. Input-only register uses immediately before
  the main loop and its declarations compile to the retained score;
  placing one at initialization scores 49.224136%.
- Retesting the second player scan's `Player` read sites on this exact
  layout, using the early opaque `player_slots` pointer for the
  follow-offset read scores 44.270832%; using it only for the swap guard
  scores 49.312140%; using it for both scores 44.403378%. The GOT reload
  at the follow-offset read is less costly than extending that alias's
  lifetime in the generated code.
- The target adds the follow-offset step from memory where the current
  build keeps `0.2f` in `xmm4`. A plain local step is optimized back to
  the retained 49.369614% code. A volatile step scores 49.111710%, a
  volatile accumulator 49.088000%, a read/write memory handoff at the
  accumulator initialization 49.321840%, and the same handoff after the
  addition 48.179960%. Forcing the local SSE load alone does not recover
  the target's surrounding XMM live ranges.
- The target loads the active `player` through `edx` before the second
  player scan, while this build uses `eax`. Giving `active_player` an
  input-only `d` register constraint at initialization or just before
  the scan scores 46.919900%; a read/write `+d` constraint scores
  47.017960% at either point. A generic read/write register handoff
  scores 45.969467%. Matching that one register explicitly disrupts
  more of the scan than it repairs.
- The target prepares the neutral, goody, and baddie list bases before
  the second player scan, while this build schedules some bases later.
  Input-only register uses before the scan for every nonempty subset
  of those three bases score 48.844467%–49.223778%. The neutral-only
  use is closest at 49.223778%; goody-only and baddie-only each score
  49.097702%. Merely advancing the base address calculations does not
  reproduce the target's stack handoff or free `edi`.
- A complete pairwise ablation of all 14 retained branch-probability
  hints on this 49.369614% layout found no higher-scoring pair. The
  best of the 91 pairs removes the path-connection-state and
  avoidance-timer hints (indices 10 and 13 in source order) and scores
  48.891520%; the best pair confined to the seven main-loop hints
  scores 48.537357%. Interactions between two hint removals do not
  explain the remaining block-order mismatch on this layout.
- Comparing displaced instruction runs shows that a 23-instruction
  interaction fragment beginning with the `ai.opponent_object` null
  check occurs around aligned row 2137 in the original and row 1457
  in the current build. This is a block-order difference, not missing
  interaction behavior. Marking the main object loop's initial
  `goto interaction` guard unlikely collapses the linked-GOT match to
  0.000000% (13,887 bytes); marking it likely scores 28.770473%
  (14,317 bytes). Inverting the predicate and hinting the complementary
  value yields the same two results. A probability hint on that guard
  cannot isolate the displaced block.
- Reordering the complete combat (`C`), path/precombat (`P`), and
  interaction (`I`) source blocks inside the main loop, with a labelled
  loop exit where needed to preserve control flow, also misses the
  target's placement. Relative to the retained `CPI` order, `PIC`
  scores 49.272630% (14,109 bytes), `CIP` 46.294900% (14,173),
  `ICP` 46.232400% (14,181), and `IPC` 46.228810% (14,181).
  The earlier `PCI` variant scored 49.366020% on this layout. Physical
  label order alone cannot reproduce the moved interaction fragment.
- The first object-loop preheader in the original stores separate
  `MiniCutCam` and `GAMEPAD_START` values, whereas the retained build
  folds the latter into a different stack slot. Retesting an explicit
  `GAMEPAD_START` local on this layout scores 49.102370% plain or with
  a read/write register handoff, 48.982758% volatile, 49.079384% with
  an input-memory use, and 48.855960% with a read/write memory use.
  The isolated stack-slot discrepancy does not justify carrying a new
  local through the loop.
- The original first-loop preheader tests `HIGHGAMEOBJECT` before
  loading `MiniCutCam`; this build hoists the local read before the test.
  Guarding the entire loop with `object_limit > 0` scores 47.110634%
  with the retained volatile camera local and 46.863148% with a plain
  local. An explicit `object_limit <= 0` early exit gives the same
  47.110634%; marking that exit unlikely raises it to 47.940730%, still
  below the retained build. Without the outer guard, a plain camera
  local scores 49.144040%, input-memory 49.150864%, read/write-memory
  49.145832%, and read/write-register 49.144040%. Moving the one load
  behind the bound test changes more preheader and loop layout than it
  fixes.
- Retesting likelihood hints for each of the first loop's three
  touch-control capability tests on this layout gives the retained
  49.369614% when marked likely; marking bit 0, bit 1, or bit 5
  unlikely scores 45.939297%, 45.940014%, or 46.078663%. The
  original's distant bit-5 update cannot be obtained by a local
  probability hint without disrupting the remaining function.
- The interaction block's own eligibility guard is another broad
  layout lever: marking the full conjunction unlikely scores
  47.757540%, marking just its first high-flags test unlikely scores
  43.119250%, and likely forms score 33.058190% or 43.306750%.
  None moves the displaced interaction fragment while preserving the
  surrounding register and block layout.
- Moving a plain `MiniCutCam` local into the first object loop at its
  sole use lets GCC hoist the global read after the positive
  `HIGHGAMEOBJECT` test. That alone scores 49.221264% (14,085 bytes)
  because the value stays in a register; a volatile local inside the
  loop scores 49.154810%. Adding an empty read/write general-register
  handoff immediately after the plain local initialization yields
  **49.385777% (14,085 bytes)**. It emits exactly one camera GOT load,
  after the bound test, and recovers the target's MiniCutCam/GAMEPAD_START
  load order. This is the retained version. A fixed `ecx` handoff scores
  49.251440%; fixed `eax`, `edx`, `esi`, or `edi` and input-only or
  post-use handoffs all score lower. Moving the read/write handoff
  earlier in the loop also scores lower (47.090157%–49.192170%). The
  useful compiler pattern is a loop-invariant read written as a local
  at the use site, with an opaque register handoff to influence its
  preheader stack home.
- On that retained camera layout, a loop-scoped `FRAMETIME` float with
  an input-only SSE register operand near the top of the first object
  loop raises 49.385777% to **49.392960%** at the same 14,085 bytes.
  The preheader now puts frame time in target register `xmm2` and zero
  in `xmm3`; without the operand those two registers are reversed.
  A plain local ties the prior score, a read/write SSE operand scores
  49.281250%, and a memory input scores 48.715520%. Moving the input
  operand among four positions before the capability updates compiles
  identically; putting it immediately before the action-frame test
  scores 49.295980%. Using the local for either or both of the two
  first-loop frame-time subtractions also compiles identically. The
  timing of the input operand, rather than the count of source reads,
  selects the useful XMM allocation.
- The original and current linked-GOT instruction streams contain the
  same set and multiplicity of calls. The only reported call-name
  difference is three `TestWalkAround` calls to compiler-generated
  clones with different `.isra` suffixes; the underlying helper is
  the same. The remaining byte and score gap is therefore dominated
  by inlined instructions, registers, and block placement rather than
  an omitted call.
- On the 49.392960% layout, caching the main object loop's
  `HIGHGAMEOBJECT` bound at four positions and trying plain, volatile,
  input-register, and read/write-register forms scores at most
  47.364940% (14,007 bytes). The target does load the bound before
  entering that loop, but a source local here changes the loop's
  wider register allocation. Repeating the avoidance-timer clamp
  experiment on this layout emits the target's `cmpnltss`/`andps`
  with SSE intrinsics or inline asm, yet scores only 49.163074%
  or at most 49.262930%, respectively, versus 49.392960% retained.
- A local `GAMEPAD_START` value inside the first loop's camera branch,
  passed through an empty read/write general-register operand, raises
  49.392960% to 49.528736%. Moving that operand after the pad pointer
  load raises it again to **49.536278%** (14,101 bytes). The build still
  emits one `GAMEPAD_START` GOT load in the target's preheader position;
  the gain comes from the later register and block lifetime. A plain
  local compiles to the prior 49.392960%, an input-only operand or a
  read/write operand outside the branch scores lower, and fixed-register
  forms score at most 49.525143%. Placing the read/write operand after
  either button-mask store also scores lower. The exact handoff position
  matters even when the global-load order is already matched.
- On the 49.536278% layout, a scoped `TouchControlsActive` boolean at
  the first-loop test has no benefit: a plain local there compiles
  identically, and register or memory handoffs score lower. Moving the
  existing `high_flags` byte declaration into the loop also compiles
  identically; volatile or wider integer forms, removing its `ecx`
  input, or making that input read/write all score lower. Those two
  stack-home mismatches are not fixed by independently changing their
  source lifetimes.
- Swapping the two independent first-loop increment expressions so
  `object` advances before `index` raises the linked-GOT score from
  49.536278% to **49.546696%** (14,101 bytes). It changes the latch
  schedule even though the target's exact `lea`/`cmp`/`jne` sequence is
  still absent. `object += 1`, `object = object + 1`, and opaque
  fixed/general-register pointer increments all compile identically
  to this retained form. Rewriting the `<` condition with reversed
  operands or equivalent Boolean spellings also compiles identically.
  Adding a positive-bound and inequality latch emits `jne` but scores
  only 49.459770%; an outer positive guard scores still lower.
- On the 49.546696% layout, clobbering `edi` at four points between
  the second player scan and the main object loop scores only
  27.501436%–27.777298%; clobbering it inside the main loop scores
  41.506466%. Making the `neutral_objects` base pointer volatile does
  move the main object into target register `edi`, but scores only
  33.420616% (13,863 bytes). Forcing that register separately still
  destroys the broader list and loop allocation.
- A fresh single-hint sweep on this layout removed and reversed each
  of the 14 retained `__builtin_expect` annotations. None exceeded
  49.546696%; the closest removal was the path-connection-state hint
  at 49.444324%. Ablating each of the 17 empty-asm markers also found
  no gain. The first-loop object and bound input markers and the
  body-entry index input can each be removed without changing the
  score; the other removals lower it.
- Moving the `active_player = player` declaration before the volatile
  `neutral_cursor` initialization raises 49.546696% to **49.581898%**
  at the same 14,101 bytes. Three tested orders with this relation emit
  byte-identical instructions; moving `active_player` among the four
  count initializations also ties. All 24 orders of the four pointer-list
  base declarations tie at 49.581898%. In contrast, putting
  `interactive_count` before `neutral_count` lowers the score to
  49.562860%, while the other tested count orders tie. The useful
  ordering constraint is the active-player load before the neutral
  cursor initialization, not a general rule to reorder every local.

- On that layout, making the goody cursor volatile as well as the
  neutral cursor raises 49.581898% to **49.645115%**, with the same
  14,101-byte symbol. All 16 volatility combinations across neutral,
  goody, interactive, and baddy cursors were measured with the GOT
  diff. Neutral plus goody is best; neutral plus goody plus baddy is
  close at 49.644756%. Volatile interactive cursor lowers the score
  to about 47.6%. Volatility changes the pointer live ranges, not the
  list contents.
- The new cursor layout shrank the stack frame to 0x560, while the
  target uses 0x570. Enlarging the output-only stack placeholder
  from 16 to 32 bytes restores 0x570 and raises the score slightly
  to **49.648710%**. Placeholder lengths 24, 28, and 36 bytes score
  49.648346%; 8, 12, and 20 stay at 0x560 and score 49.644756%.
  Larger lengths grow the frame beyond the target. The first-loop
  scratch homes nevertheless stay at esp+0xb4 through esp+0xd8
  versus the target's esp+0xfc through esp+0x118; matching total
  frame size alone does not align these homes.
- An extra addressable 16–80-byte local before the first object loop
  changes only the frame size, leaving its scratch homes at their
  current offsets. Putting that local inside the loop moves one home
  but lowers the score to 47.623560%. Rewriting the capability update
  with explicit old/new locals compiles identically for the first
  test; using the old local for all three tests changes allocation and
  scores 49.644398%. Empty memory handoffs for neutral or goody
  cursors at the scan-to-main-loop boundary tie the retained score;
  forcing their base pointers into memory or pinning a GOT address to
  edi there lowers it substantially. The target carries the main
  object in edi; the current build carries it in esi because the
  scan setup uses edi for the neutral list base. Forcing edi locally
  has not recreated the target's larger register allocation.

- Copying the drop-back timer into a plain float local immediately after
  AITriggerSetSysProcess and using that copy in the second player scan
  raises 49.648710% to **49.732040%**, still 14,101 bytes with a
  0x570 frame. Placing the declaration before the pointer arrays or
  before the padding ties; later placements score 49.587643%–49.648710%.
  An empty SSE-register handoff at the winning location scores
  49.708690%, while memory handoffs score 49.507904%. Scoped locals
  can improve code generation through placement alone; an extra
  register constraint can reverse the gain.
- The target reads both Player[1] and Player[index] through the
  preserved Player-array base during the second scan, while this
  build reloads the global on those paths. Replacing either source
  read with the existing player_slots cache does remove the local
  reload but perturbs whole-function allocation: replacing Player[1]
  scores 49.585130%, replacing Player[index] scores 44.659122%, and
  replacing both scores 44.655533%. A locally matching load is not
  sufficient evidence to retain an edit in this large function.
  Removing the earlier read/write register marker for player_slots
  scores 47.894398%; changing it to read-only register, fixed esi,
  or memory forms scores 46.315730%–48.690372%. Combining the
  marker changes with the cached reads does not improve those
  variants. The earlier marker likely blocks some common-subexpression
  reuse, but it is still required for the current wider allocation.
- The jump-stuck branch is a large block-placement mismatch. The
  target loads jump_stuck_time at function offset 0x160b, while the
  retained build puts that load at 0x33f7. Changing its existing
  unlikely path-connection hint to likely moves the load to 0x1582,
  close to the target position, but drops the whole match to
  47.487427%; removing the hint puts it at 0x2075 and scores
  49.629670%. With the path hint set to likely, removing or
  reversing each of the other 13 retained branch hints also fails
  to beat 49.732040% (best tested pair: 47.649067%). The hint
  controls a major cold-block partition, but moving this one block
  alone displaces too much correctly aligned code. Revisit the
  surrounding control-flow structure rather than treating its
  position as an isolated score target.

## NDK r8e GCC 4.7 trace ordering

The NDK r8e x86 toolchain compiles this file with GCC 4.7 at `-O3`.
For this investigation, `bb-reorder.c` from the Android GCC 4.7 tree's
March 2013 snapshot is byte-identical to upstream GCC 4.7.2's copy.
The precise source revision used to build the shipped NDK compiler has
not been established. Relevant source files are
[Android GCC 4.7 `bb-reorder.c`](https://android.googlesource.com/toolchain/gcc/+/e88c4f52a771a323dfad124e5f4836c2504a0905/gcc-4.7/gcc/bb-reorder.c)
and [its `predict.c`](https://android.googlesource.com/toolchain/gcc/+/e88c4f52a771a323dfad124e5f4836c2504a0905/gcc-4.7/gcc/predict.c).
The predictor table is in [Android GCC 4.7 `predict.def`](https://android.googlesource.com/toolchain/gcc/+/e88c4f52a771a323dfad124e5f4836c2504a0905/gcc-4.7/gcc/predict.def).

- The RTL block reordering pass greedily forms traces in several rounds.
  Its successive branch-probability thresholds are 40%, 20%, 10%, 0%,
  0%, and its execution-frequency thresholds are 50%, 20%, 5%, 0%,
  0% relative to the function entry frequency. A branch hint can
  therefore change which round contains an entire successor region.
- `better_edge_p` treats edge probabilities within roughly 10% of its
  current best as close. It then compares the successor frequencies,
  favoring a *lower* frequency when the difference is significant;
  if those are also close, it favors the successor that immediately
  followed this block before reordering. Thus source/GIMPLE block order
  matters particularly for ties. Blocks ending in a call are a special
  case: trace formation prefers their fallthrough edge.
- `predict.c` combines branch predictors, but a predictor marked
  first-match can override the combined estimate. `__builtin_expect`
  becomes a predictor on the relevant GIMPLE edge; it is not merely
  a local `likely`/`unlikely` annotation for instruction selection.
  When testing a hint, inspect the resulting `.bbro` dump's edge
  frequencies, trace rounds, and final order, in addition to the final
  disassembly.
- This GCC 4.7 `predict.def` defines `PRED_BUILTIN_EXPECT` as a
  first-match predictor with `PROB_VERY_LIKELY`. In `predict.c`, that
  is `REG_BR_PROB_BASE - (REG_BR_PROB_BASE / 2000 - 1)`, about 99.96%
  for a 10,000-unit base. It is much stronger than the no-hint tree
  opcode-positive predictor, whose hit rate is 73%. A likely/unlikely
  flip can therefore send a whole trace into the last round rather
  than merely exchanging two adjacent blocks.
- Use the actual NDK compiler with `-fdump-rtl-bbro-details` to capture
  the pass decisions. The local compile action can be retrieved with
  `bazelisk aquery --config=target 'inputs(".*gameobjects.cpp", mnemonic("CppCompile", deps(//src:saga_target)))' --output=jsonproto`.
  Redirect its object and dependency outputs before adding the dump flag
  so the diagnostic compile does not replace Bazel's target artifact.

The jump-stuck branch at source line 2698 is an example of an unhinted
50/50 branch in the retained `.bbro` dump. Permuting its three
side-effect-free conjuncts changes the initial CFG and hence the trace
layout. Of 18 combinations of predicate order and the old state-test
hint, the best evaluates `movement_stuck_time > jump_stuck_time`, then
`path_info.flags & 1`, then `path_connection_state == 0`, without a
hint. This raises the GOT-aware whole-function score from 49.732040%
to **49.945404%**. The symbol becomes 14,116 bytes and retains a
0x570 stack frame. The `jump_stuck_time` load moves from current
offset 0x33f7 to 0x1568 (target 0x160b). Retesting single retained
hint changes on this new layout found no further improvement.
All 16 volatile/plain combinations of the four list cursors were also
remeasured after this reorder. The retained volatile neutral and goody
cursors still win; the closest alternative additionally makes the
baddy cursor volatile and scores 49.941810%.

The call skeleton exposes a larger trace discrepancy immediately after
`AISysProcessCharacter`: the target's next calls include
`BoltType_FindByID`, `SetObjAsHeadTarget`, and then later movement/path
helpers; this build brings the wall-shuffle `NuVecSub`/`NuVecNorm`/
`NuVecRotateY` calls forward and leaves `BoltType_FindByID` much later.
The target's call offsets are 0x0fab for `AISysProcessCharacter`,
0x103c for `BoltType_FindByID`, and 0x1ff4 for the wall-shuffle
`NuVecSub`; the retained build puts them at 0x0f9f, 0x281f, and
0x10cf respectively. A direct source-flow swap to execute the combat
section before movement scores only 46.361350%. Trying likely/unlikely
hints on its outer flags test and opponent test scores still lower
(best tested 42.941093% among those hinted variants). These call
positions show that layout is a major problem, but simply swapping
the two logical phases does not reconstruct the target's trace graph.

The branch after `AISysProcessCharacter` gives a more precise CFG
constraint. At target offset 0x0fb6 it tests `flags_low` and at 0x0fbd
branches to the early movement checks at 0x1548 when bit 0x80 is clear.
When that bit is set, execution falls through to the later FreePlay
and route checks at 0x0fc3, followed by combat. The retained build
instead uses `js` at 0x0fb1 to skip early movement; its early checks
fall through immediately, and the later route checks are elsewhere.
The source's broad ordering is semantically consistent with the
target, but the compiler has selected the other edge as its main
trace. This is a trace-layout mismatch, not evidence that combat
should run before movement.

In a diagnostic compile with the outer `flags_low` hint removed,
the `.bbro` dump assigns 27% to the branch that skips early movement
and 73% to the early movement fallthrough; the whole-function score
is 43.545258%. Reversing the retained hint makes the route checks
fall through, but defers early movement too far (around 0x2c00) and
scores 39.576508%. A source-level out-of-line early-movement block,
equivalent goto spellings, and local register/volatile barriers did
not retain a better layout. An equivalent unsigned-subtraction test
puts the FreePlay check immediately after the call, but adds a test
instruction and scores 49.336210%; combining it with FreePlay hints
peaks at 49.909480%, still below the retained build. Changing the
outer hint jointly with the adjacent action-target, JEDI_B, or
distance hints also stayed below 49.945404%. The next candidate must
adjust the edge frequencies or block graph without turning the early
movement successor into a nearly zero-frequency cold trace.
An empty `asm goto` edge to the early-movement label raises the
no-hint score to 44.867817%, but lowers the retained likely-hint
layout to 49.941810% and the unlikely-hint layout to 27.376797%.
Its artificial edge changes trace frequencies without reproducing
the target layout, so it is not retained.

Further register and wall-shuffle probes on the 49.945404% layout:

- The first-loop `Obj` load uses `edi` in the target but `esi` in the
  current build; both compilers then use `eax` for the object itself
  and `edi` for the `HIGHGAMEOBJECT` bound. An entire-function local
  register declaration pinned to `edi` reproduces that first load but
  lowers the whole score to 45.005750%. A fixed-`edi` empty asm at
  the first-loop entry scores 47.715157%. Scoping the pointer to just
  the first loop lets GCC return to `esi`; the best scoped variant
  ties the retained score. The disagreement is in live ranges and
  scheduling, not a single wrong pointer register inside the loop.
- At the main-loop preheader, the target loads `HIGHGAMEOBJECT` and
  `Obj` before storing the final `party_under_cover` value, while the
  current build stores that value first and reuses `esi` for the two
  global loads. Caching the cover value in a local across the second
  player scan and deferring its store until after `Obj` is loaded does
  move the load ahead of the store, but scores 49.515446%. Deferring
  until after an explicit bound load scores 47.708690%. This local
  scheduling improvement does not recreate the target register
  allocation.
- All six permutations of the three `WALLSHUFFLE` bitwise-AND
  operands compile identically. The target branches to the
  wall-shuffle `NuVecSub` block later (call offset 0x1ff4), while
  the retained build falls through to it at 0x10cf. Hinting the
  inner traversal-mask test as unlikely moves the call to 0x24cd
  but scores only 42.257904%; hinting the connection pointer as
  unlikely moves it to 0x249d and scores 42.588722%. Combining
  either with a plain or reversed outer `flags_low` hint also stays
  below the retained score. A matching isolated call position is
  insufficient when the surrounding trace graph moves differently.

- Loading `&Obj` and `&HIGHGAMEOBJECT` through address pointers
  constrained to `edi` and `edx` reproduces the target's first four
  preheader instructions exactly: the two GOT loads followed by the
  two dereferences. The whole score nevertheless drops to 49.230960%
  because the constraint changes register allocation in the second
  player scan and call setup around offsets 0x0c00–0x0e00. Combining
  that variant with all 16 volatile/plain cursor choices peaks at
  49.411278%, still below the retained layout. Similarly, constraining
  the four entry global addresses to `eax`/`edi`/`edx`/`esi` exactly
  reproduces the target call preheader but scores at most 48.161278%
  as a standalone variant. An exact local instruction window can be
  outweighed by longer live ranges later in this function.

Moving the wall-shuffle body behind an explicit label while leaving
the stuck-time test as the immediate fallthrough raises the GOT-aware
score from 49.945404% to **50.121770%**; the current symbol shrinks
from 14,116 to 14,108 bytes. The two branches are semantically the
same as the original `if`/`else if`. Inspection of the disassembly
shows that the wall-shuffle `NuVecSub` call remains at 0x10cf, so this
is not the sought block-order breakthrough. The main change is that
GCC removes a redundant `FreePlay` GOT load and read in a cold block
near the function end, with a few other register-choice changes.
Putting the wall-shuffle label after the route code instead scores
48.888650%; marking that moved label cold makes no difference.
The same full-binary report rises from 59.250828% to 59.251370%
after this retained edit, compared with 59.164350% on clean main.

With the wall-shuffle label in place, retesting all 18 permutations
of the stuck-time, path-flag, and connection-state predicates (with
plain/likely/unlikely state) found no improvement over the
time–flag–state order. The target disassembly tests state, flag,
then timer, but spelling the source that way scores 49.806034% on
this layout and places the timer load at 0x2075 instead of the
retained 0x1568 (target 0x160b). Inverting the stuck condition into
an explicit jump over its body raises **50.121770% to 50.164513%**
without changing symbol size (14,108 bytes). This edit moves a
`FreePlay` GOT load and read from one unmatched cold block around
0x2d8c to another around 0x3705; it does not yet fix wall-shuffle
placement. The whole-binary fuzzy score becomes 59.251503%.

The last retained build measured 50.164513% (original 14,477 bytes;
current 14,108 bytes). It is still far from an instruction match.
Rebuild and measure again after every edit; do not treat the figures
above as the score of a later revision.

An explicit dispatch immediately after `AISysProcessCharacter` was
also tested on this retained layout. The dispatch sent objects with
bit `0x80` clear directly to the movement label and other objects to
the route label. Keeping route after movement scored 49.930676% at
best (a likely movement branch); placing route before movement scored
48.911636% at best. Plain and reversed hints scored lower. This
confirms that the missing target trace cannot be recovered by adding
only that entry test: the extra CFG edge changes the compiler's trace
partition. Removing the now-redundant outer movement guard produces
the same scores and symbol sizes, showing that GCC had already folded
the repeated condition. Rechecking the five newer retained hints
(head target, Jango level, cover end, paired timers, and wall scale)
in both plain and reversed forms also found no improvement; the best
variant scored 49.874640% with the paired-timer hint removed. The
retained source remains at 50.164513%.

The retained NDK r8e GCC `.bbro` dump makes the branch pressure
concrete: the `flags_low` test after `AISysProcessCharacter` assigns
the edge that skips movement `REG_BR_PROB 4`, on GCC's 10,000-point
scale. The retained binary thus keeps movement immediately after the
call, while the target branches to movement at offset `0x1548` and
falls through to `FreePlay`/route and combat at `0x0fc3`. Reordering
the three source sections (combat, movement, route) in all six
permutations, with explicit labels and equivalent jumps, peaked at
50.085846% for movement–combat–route; each other ordering stayed
below the retained 50.164513%. A likely or unlikely hint on either
the route's `FreePlay` test or its suit bit, combined with all three
outer movement-hint states, also stayed below the retained score.
An inverted early-exit spelling of the outer movement guard compiled
to the same scores as the earlier explicit entry dispatch. Finally,
removing or reversing each of the function's 13 retained
`__builtin_expect` calls individually found no gain on this layout;
the best ablation scored 49.874640%. This is evidence that the main
remaining issue is the connected trace graph and live ranges, rather
than one mistuned hint or the physical source order of these sections.

The early call sequence gives another trace-order constraint. Target
offsets after `AISysProcessCharacter` are `BoltType_FindByID` at
`0x103c`, `SetObjAsHeadTarget` at `0x1207`, and the post-loop
`LEGO_AISysCreatureInteraction2D` at `0x1261`; the retained build has
the wall-ray helpers around `0x10cf` and puts these calls much later.
Hinting the combat flag `field_0xef8 & 0x80` likely together with an
unlikely movement guard moves `BoltType_FindByID` to `0x101e`.
Making the head-target guard likely also moves that call to `0x11fd`.
The joint whole-symbol score is only 33.974500%, because the
post-loop interaction call stays at `0x1928` and many other blocks
and registers diverge. All four nontrivial reorderings of combat,
precombat, and interaction sections on this joint variant stay below
33.93%. Hinting the main object-loop condition or the interaction
mode changes the generated loop substantially and scores at most
27.660202% among eight joint variants. The two near-exact call
offsets are therefore insufficient evidence of a better layout.
The target jumps back to the main loop immediately after the
head-target call; its physically next block is a separate post-loop
trace. A later attempt should study which trace GCC selects at that
boundary, rather than treating the two calls as a fallthrough chain.
