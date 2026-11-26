# Documentação

Índice dos documentos. Os textos técnicos estão em inglês, como o código. Os
nomes seguem o padrão do projeto original: `<área>_<rodada>.md` registra o que
uma rodada de trabalho mudou e mediu (por exemplo `collision_s2.md` é a rodada
S2 da colisão), e `<área>_map.md` é o mapa de uma área do jogo.

## Comece por aqui

| Documento | Sobre |
| --- | --- |
| [arquitetura.md](arquitetura.md) | as peças do port e como se encaixam (português) |
| [STATUS.md](STATUS.md) | onde o port está, plataforma por plataforma |
| [known_issues.md](known_issues.md) | problemas conhecidos |
| [worklist.md](worklist.md) | lista de trabalho em aberto |
| [runtime_switches.md](runtime_switches.md) | todas as chaves de runtime (`PS2X_*`, `SOTC_*`) |
| [texture_packs.md](texture_packs.md) | trocar texturas por PNGs |

## Mapas das áreas do jogo

| Documento | Área |
| --- | --- |
| [native_renderer_map.md](native_renderer_map.md) | cada efeito do renderizador nativo e a função do jogo que ele substitui |
| [display_map.md](display_map.md) | display e modelos |
| [characters_map.md](characters_map.md) | personagens e animação |
| [collision_map.md](collision_map.md) | colisão e física |
| [particles_map.md](particles_map.md) | partículas |
| [vu1_routines.md](vu1_routines.md) | rotinas de VU1 do jogo |

## Descompilação

| Documento | Sobre |
| --- | --- |
| [decomp.md](decomp.md) | método e histórico completo (arquivo grande) |
| [decomp_100_plan.md](decomp_100_plan.md) | plano para chegar a 100% do código nativo |
| `decomp_a1.md` a `decomp_a15.md` | rodadas A: o lifter tipado (`tools/typed_lift2.py`) |
| `decomp_b1.md` a `decomp_b10.md` | rodadas B: as threads do jogo em threads do host |
| `decomp_d1.md` a `decomp_d3.md` | rodadas D: corpos nativos das funções escritas à mão |
| `decomp_w1.md` a `decomp_w3.md` | rodadas W: o display e os personagens no estilo do Vita |
| `decomp_dc1.md`, `decomp_dc2.md` | a cadeia de personagens em C++ |
| `decomp_c1.md`, `decomp_g1.md`, `decomp_i1.md`, `decomp_i2.md`, `decomp_k1.md`, `decomp_k2.md`, `decomp_m1.md`, `decomp_p2.md`, `decomp_p3.md`, `decomp_r1.md` | rodadas de console: núcleos, vitaGL, cache de instruções, módulo de kernel, memória, profiler |
| [a9_sweep.md](a9_sweep.md), [ab_s20.md](ab_s20.md) | custos do código gerado e A/B das chaves desligadas |

## Redesenhos por subsistema (rodadas S)

| Área | Documentos |
| --- | --- |
| Partículas | [particles_s1.md](particles_s1.md), [particles_s5.md](particles_s5.md), [ptcl_s22.md](ptcl_s22.md) |
| Colisão | [collision_s2.md](collision_s2.md), [collision_s6.md](collision_s6.md) |
| Personagens e animação | [characters_s3.md](characters_s3.md), [characters_s8.md](characters_s8.md), [motion_s13.md](motion_s13.md) |
| Display | [display_s4.md](display_s4.md), [display_s10.md](display_s10.md), [display_s12.md](display_s12.md), [display_s14.md](display_s14.md), [display_s16.md](display_s16.md) |
| Luzes | [lights_s19.md](lights_s19.md), [lights_s21.md](lights_s21.md), [lights_s23.md](lights_s23.md) |
| Vento | [wind_s7.md](wind_s7.md) |
| Runtime | [runtime_s9.md](runtime_s9.md), [runtime_s11.md](runtime_s11.md), [runtime_s17.md](runtime_s17.md), [runtime_s18.md](runtime_s18.md) |

## Caminho nativo e desempenho

| Documento | Sobre |
| --- | --- |
| [native_port_plan.md](native_port_plan.md), [native_port_log.md](native_port_log.md) | plano e diário do port nativo |
| [native_all_plan.md](native_all_plan.md) | as camadas com forma de PS2 que ainda restam |
| `native_n3.md` a `native_n6.md`, [native_r1.md](native_r1.md) | rodadas N: float nativo, convenção de chamada, threads e scheduler |
| [native_audio_plan.md](native_audio_plan.md), [sound_n2.md](sound_n2.md) | som nativo |
| `perf_p1.md` a `perf_p4.md` | rodadas de desempenho para o Vita |
| [vu1_fast_recompiler.md](vu1_fast_recompiler.md), [vu1_gpu_path.md](vu1_gpu_path.md) | planos para o VU1 |

## PS Vita

| Documento | Sobre |
| --- | --- |
| [vita_perf_guide.md](vita_perf_guide.md) | guia de desempenho para código preso em CPU e memória |
| [vita_suspend.md](vita_suspend.md) | sair do jogo e voltar |
| [console_profile_2026-11-20.md](console_profile_2026-11-20.md), [console_c1.md](console_c1.md), [console_c2.md](console_c2.md) | perfis no console |

## Dados gerados

Não edite à mão: as ferramentas em `tools/` reescrevem estes arquivos.

| Arquivo | Gerado por |
| --- | --- |
| `port_status.json`, `port_status.svg` | dados do port; `tools/status_treemap.py` desenha o SVG |
| `decomp_status.json`, `decomp_status.svg` | `tools/decomp_status.py` |
| `decomp_history.json`, `decomp_history.svg` | `tools/decomp_history.py` |
| `decomp_tree.json` | `tools/decomp_tree.py` |
| `rounds.json` | histórico das rodadas |
| `*_typed.txt`, `*_natives.txt`, `n4b_native.txt`, `n5_*.txt` | listas de funções de cada rodada |
| `index.html`, `assets/`, `img/`, `vendor/` | página de progresso (GitHub Pages) |
