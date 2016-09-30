/*
 *	Logger Library by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A generic logger library providing application execution reports.
 *
 */

/* -- Headers -- */

#include <log/log_aspect_format.h>
#include <log/log_policy_format.h>

/* -- Member Data -- */

struct log_aspect_format_buffer_type
{
	void * data;
	size_t size;
};

struct log_aspect_format_serialize_type
{
	log_record record;
	struct log_aspect_format_buffer_type buffer;
};

struct log_aspect_format_deserialize_type
{
	log_record record;
	struct log_aspect_format_buffer_type buffer;
};

/* -- Private Methods -- */

LOG_NO_EXPORT static log_aspect_data log_aspect_format_create(log_aspect aspect, const log_aspect_ctor ctor);

LOG_NO_EXPORT static int log_aspect_format_impl_serialize_cb(log_aspect aspect, log_policy policy, log_aspect_notify_data notify_data);

LOG_NO_EXPORT static int log_aspect_format_impl_serialize(log_aspect aspect, log_record record, void * buffer, const size_t size);

LOG_NO_EXPORT static int log_aspect_format_impl_deserialize_cb(log_aspect aspect, log_policy policy, log_aspect_notify_data notify_data);

LOG_NO_EXPORT static int log_aspect_format_impl_deserialize(log_aspect aspect, log_record record, void * buffer, const size_t size);

LOG_NO_EXPORT static int log_aspect_format_destroy(log_aspect aspect);

/* -- Methods -- */

log_aspect_interface log_aspect_format_interface()
{
	static struct log_aspect_format_impl_type log_aspect_format_impl =
	{
		&log_aspect_format_impl_serialize,
		&log_aspect_format_impl_deserialize
	};

	static struct log_aspect_interface_type aspect_interface_format =
	{
		&log_aspect_format_create,
		&log_aspect_format_impl,
		&log_aspect_format_destroy
	};

	return &aspect_interface_format;
}

static log_aspect_data log_aspect_format_create(log_aspect aspect, const log_aspect_ctor ctor)
{
	/* TODO */
	(void)aspect;
	(void)ctor;

	return NULL;
}

static int log_aspect_format_impl_serialize_cb(log_aspect aspect, log_policy policy, log_aspect_notify_data notify_data)
{
	struct log_aspect_format_serialize_type * serialize_args = notify_data;

	log_policy_format_impl format_impl = log_policy_derived(policy);

	size_t written_size = format_impl->serialize(policy, serialize_args->record, serialize_args->buffer.data, serialize_args->buffer.size);

	(void)aspect;

	return (written_size != serialize_args->buffer.size);
}

static int log_aspect_format_impl_serialize(log_aspect aspect, log_record record, void * buffer, const size_t size)
{
	struct log_aspect_format_serialize_type notify_data;

	notify_data.record = record;
	notify_data.buffer.data = buffer;
	notify_data.buffer.size = size;

	return log_aspect_notify_all(aspect, &log_aspect_format_impl_serialize_cb, (log_aspect_notify_data)&notify_data);
}

static int log_aspect_format_impl_deserialize_cb(log_aspect aspect, log_policy policy, log_aspect_notify_data notify_data)
{
	struct log_aspect_format_deserialize_type * deserialize_args = notify_data;

	log_policy_format_impl format_impl = log_policy_derived(policy);

	size_t size = format_impl->deserialize(policy, deserialize_args->record, deserialize_args->buffer.data, deserialize_args->buffer.size);

	(void)aspect;

	return (deserialize_args->buffer.size != size);
}

static int log_aspect_format_impl_deserialize(log_aspect aspect, log_record record, void * buffer, const size_t size)
{
	struct log_aspect_format_deserialize_type notify_data;

	notify_data.record = record;
	notify_data.buffer.data = buffer;
	notify_data.buffer.size = size;

	return log_aspect_notify_all(aspect, &log_aspect_format_impl_deserialize_cb, (log_aspect_notify_data)&notify_data);
}

static int log_aspect_format_destroy(log_aspect aspect)
{
	/* TODO */
	(void)aspect;

	return 0;
}
