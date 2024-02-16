// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/16.

#include <memory>

#include "seen/base/logger.h"
#include "seen/runtime/wasm_ext.h"

namespace seen::runtime {

namespace {

// Copy from wasm-micro-runtime/core/iwasm/common/wasm_runtime_common.c:@parse_args_to_uint32_array
void WriteUint32Array(const wasm_val_t* args, std::size_t arg_count, std::uint32_t* out_argv) {
  std::uint32_t ptr = 0;
  for (std::uint32_t i = 0; i < arg_count; i++) {
    switch (args[i].kind) {
      case WASM_I32:
        out_argv[ptr++] = args[i].of.i32;
        break;
      case WASM_I64: {
        union {
          std::uint64_t val;
          std::uint32_t parts[2];
        } unions;
        unions.val = args[i].of.i64;
        out_argv[ptr++] = unions.parts[0];
        out_argv[ptr++] = unions.parts[1];
        break;
      }
      case WASM_F32: {
        union {
          float val;
          std::uint32_t part;
        } unions;
        unions.val = args[i].of.f32;
        out_argv[ptr++] = unions.part;
        break;
      }
      case WASM_F64: {
        union {
          double val;
          std::uint32_t parts[2];
        } unions;
        unions.val = args[i].of.f64;
        out_argv[ptr++] = unions.parts[0];
        out_argv[ptr++] = unions.parts[1];
        break;
      }
      case WASM_FUNCREF: {
        out_argv[ptr++] = args[i].of.i32;
        break;
      }
      case WASM_ANYREF: {
#if UINTPTR_MAX == UINT32_MAX
        out_argv[ptr++] = args[i].of.foreign;
#else
        union {
          std::uintptr_t val;
          std::uint32_t parts[2];
        } unions;

        unions.val = static_cast<std::uintptr_t>(args[i].of.foreign);
        out_argv[ptr++] = unions.parts[0];
        out_argv[ptr++] = unions.parts[1];
#endif
        break;
      }
      default:
        SEEN_ASSERT(false);
        break;
    }
  }
}

// Copy from wasm-micro-runtime/core/iwasm/common/wasm_runtime_common.c:@parse_uint32_array_to_results
void WriteWASMResults(const std::uint32_t* argv,
                      std::size_t result_count,
                      const ValueType* result_types,
                      wasm_val_t* out_results) {
  std::uint32_t ptr;
  for (std::uint32_t i = 0; i < result_count; i++) {
    switch (result_types[i]) {
      case ValueType::kI32:
        out_results[i].kind = WASM_I32;
        out_results[i].of.i32 = static_cast<std::int32_t>(argv[ptr++]);
        break;
      case ValueType::kI64: {
        union {
          std::uint64_t val;
          std::uint32_t parts[2];
        } unions;
        unions.parts[0] = argv[ptr++];
        unions.parts[1] = argv[ptr++];
        out_results[i].kind = WASM_I64;
        out_results[i].of.i64 = static_cast<std::int64_t>(unions.val);
        break;
      }
      case ValueType::kF32: {
        union {
          float val;
          std::uint32_t part;
        } unions;
        unions.part = argv[ptr++];
        out_results[i].kind = WASM_F32;
        out_results[i].of.f32 = unions.val;
        break;
      }
      case ValueType::kF64: {
        union {
          double val;
          std::uint32_t parts[2];
        } unions;
        unions.parts[0] = argv[ptr++];
        unions.parts[1] = argv[ptr++];
        out_results[i].kind = WASM_F64;
        out_results[i].of.f64 = unions.val;
        break;
      }
      case ValueType::kFuncRef: {
        out_results[i].kind = WASM_I32;
        out_results[i].of.i32 = static_cast<std::int32_t>(argv[ptr++]);
        break;
      }
      case ValueType::kExternRef: {
#if UINTPTR_MAX == UINT32_MAX
        out_results[i].kind = WASM_ANYREF;
        out_results[i].of.foreign = static_cast<std::uintptr_t>(argv[ptr++]);
#else
        union {
          std::uintptr_t val;
          std::uint32_t parts[2];
        } unions;
        unions.parts[0] = argv[ptr++];
        unions.parts[1] = argv[ptr++];
        out_results[i].kind = WASM_ANYREF;
        out_results[i].of.foreign = unions.val;
#endif
        break;
      }
      default:
        SEEN_ASSERT(false);
        break;
    }
  }
}

}  // namespace

void wasm_runtime_call_indirect_a(wasm_exec_env_t exec_env,
                                  std::uint32_t function_id,
                                  std::uint32_t result_count,
                                  const ValueType* result_types,
                                  std::uint32_t arg_count,
                                  const wasm_val_t* args,
                                  wasm_val_t* out_results) {
  auto argv = std::make_shared<std::uint32_t>(arg_count + result_count);
  WriteUint32Array(args, arg_count, argv.get());
  wasm_runtime_call_indirect(exec_env, function_id, arg_count, argv.get());
  WriteWASMResults(argv.get(), result_count, result_types, out_results);
}

void wasm_runtime_call_indirect_a(wasm_exec_env_t exec_env,
                                  std::uint32_t function_id,
                                  std::uint32_t arg_count,
                                  const wasm_val_t* args) {
  wasm_runtime_call_indirect_a(exec_env, function_id, 0, nullptr, arg_count, args, nullptr);
}

}  // namespace seen::runtime
