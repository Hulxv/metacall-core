/*
 *	Loader Library by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A plugin for loading net code at run-time into a process.
 *
 */

#ifndef CS_LOADER_IMPL_H
#define CS_LOADER_IMPL_H 1

#include <cs_loader/cs_loader_api.h>

#include <loader/loader_impl_interface.h>

#ifdef __cplusplus
extern "C" {
#endif

CS_LOADER_API loader_impl_data cs_loader_impl_initialize(loader_impl impl);

CS_LOADER_API int cs_loader_impl_execution_path(loader_impl impl, const loader_naming_path path);

CS_LOADER_API loader_handle cs_loader_impl_load(loader_impl impl, const loader_naming_path path, loader_naming_name name);

CS_LOADER_API int cs_loader_impl_clear(loader_impl impl, loader_handle handle);

CS_LOADER_API int cs_loader_impl_discover(loader_impl impl, loader_handle handle, context ctx);

CS_LOADER_API int cs_loader_impl_destroy(loader_impl impl);

#ifdef __cplusplus
}
#endif

#endif /* CS_LOADER_IMPL_H */
