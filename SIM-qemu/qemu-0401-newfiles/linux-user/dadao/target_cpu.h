#ifndef DADAO_TARGET_CPU_H
#define DADAO_TARGET_CPU_H

static inline void cpu_clone_regs(CPUDADAOState *env, target_ulong newsp)
{
    if (newsp) {
        env->regpc = newsp;
    }

    env->regpc = 0;
}

static inline void cpu_set_tls(CPUDADAOState *env, target_ulong newtls)
{
    env->regpc = newtls;
}

static inline abi_ulong get_sp_from_cpustate(CPUDADAOState *env)
{
   return env->regpc;
}

#endif