# Upstream API delta — hbmongoc vs mongo-c-driver

Generated 2026-10-07 from `mongo-c-driver` tags `1.30.12` and `2.5.5` (public headers, `-private.h` excluded).

- public API 1.30.12: **1023** | covered **153** (15.0%) | missing **870**
- public API 2.5.5   : **1038** | covered **149** (14.4%) | missing **889**

## Missing vs 1.30.12, grouped by header

### mongoc-apm.h (82)

`mongoc_apm_callbacks_destroy`, `mongoc_apm_callbacks_new`, `mongoc_apm_command_failed_get_command_name`, `mongoc_apm_command_failed_get_context`, `mongoc_apm_command_failed_get_database_name`, `mongoc_apm_command_failed_get_duration`, `mongoc_apm_command_failed_get_error`, `mongoc_apm_command_failed_get_host`, `mongoc_apm_command_failed_get_operation_id`, `mongoc_apm_command_failed_get_reply`, `mongoc_apm_command_failed_get_request_id`, `mongoc_apm_command_failed_get_server_connection_id`, `mongoc_apm_command_failed_get_server_connection_id_int64`, `mongoc_apm_command_failed_get_server_id`, `mongoc_apm_command_failed_get_service_id`, `mongoc_apm_command_started_get_command`, `mongoc_apm_command_started_get_command_name`, `mongoc_apm_command_started_get_context`, `mongoc_apm_command_started_get_database_name`, `mongoc_apm_command_started_get_host`, `mongoc_apm_command_started_get_operation_id`, `mongoc_apm_command_started_get_request_id`, `mongoc_apm_command_started_get_server_connection_id`, `mongoc_apm_command_started_get_server_connection_id_int64`, `mongoc_apm_command_started_get_server_id`, `mongoc_apm_command_started_get_service_id`, `mongoc_apm_command_succeeded_get_command_name`, `mongoc_apm_command_succeeded_get_context`, `mongoc_apm_command_succeeded_get_database_name`, `mongoc_apm_command_succeeded_get_duration`, `mongoc_apm_command_succeeded_get_host`, `mongoc_apm_command_succeeded_get_operation_id`, `mongoc_apm_command_succeeded_get_reply`, `mongoc_apm_command_succeeded_get_request_id`, `mongoc_apm_command_succeeded_get_server_connection_id`, `mongoc_apm_command_succeeded_get_server_connection_id_int64`, `mongoc_apm_command_succeeded_get_server_id`, `mongoc_apm_command_succeeded_get_service_id`, `mongoc_apm_server_changed_get_context`, `mongoc_apm_server_changed_get_host`, `mongoc_apm_server_changed_get_new_description`, `mongoc_apm_server_changed_get_previous_description`, `mongoc_apm_server_changed_get_topology_id`, `mongoc_apm_server_closed_get_context`, `mongoc_apm_server_closed_get_host`, `mongoc_apm_server_closed_get_topology_id`, `mongoc_apm_server_heartbeat_failed_get_awaited`, `mongoc_apm_server_heartbeat_failed_get_context`, `mongoc_apm_server_heartbeat_failed_get_duration`, `mongoc_apm_server_heartbeat_failed_get_error`, `mongoc_apm_server_heartbeat_failed_get_host`, `mongoc_apm_server_heartbeat_started_get_awaited`, `mongoc_apm_server_heartbeat_started_get_context`, `mongoc_apm_server_heartbeat_started_get_host`, `mongoc_apm_server_heartbeat_succeeded_get_awaited`, `mongoc_apm_server_heartbeat_succeeded_get_context`, `mongoc_apm_server_heartbeat_succeeded_get_duration`, `mongoc_apm_server_heartbeat_succeeded_get_host`, `mongoc_apm_server_heartbeat_succeeded_get_reply`, `mongoc_apm_server_opening_get_context`, `mongoc_apm_server_opening_get_host`, `mongoc_apm_server_opening_get_topology_id`, `mongoc_apm_set_command_failed_cb`, `mongoc_apm_set_command_started_cb`, `mongoc_apm_set_command_succeeded_cb`, `mongoc_apm_set_server_changed_cb`, `mongoc_apm_set_server_closed_cb`, `mongoc_apm_set_server_heartbeat_failed_cb`, `mongoc_apm_set_server_heartbeat_started_cb`, `mongoc_apm_set_server_heartbeat_succeeded_cb`, `mongoc_apm_set_server_opening_cb`, `mongoc_apm_set_topology_changed_cb`, `mongoc_apm_set_topology_closed_cb`, `mongoc_apm_set_topology_opening_cb`, `mongoc_apm_topology_changed_get_context`, `mongoc_apm_topology_changed_get_new_description`, `mongoc_apm_topology_changed_get_previous_description`, `mongoc_apm_topology_changed_get_topology_id`, `mongoc_apm_topology_closed_get_context`, `mongoc_apm_topology_closed_get_topology_id`, `mongoc_apm_topology_opening_get_context`, `mongoc_apm_topology_opening_get_topology_id`

### mongoc-bulkwrite.h (66)

`mongoc_bulkwrite_append_deletemany`, `mongoc_bulkwrite_append_deleteone`, `mongoc_bulkwrite_append_insertone`, `mongoc_bulkwrite_append_replaceone`, `mongoc_bulkwrite_append_updatemany`, `mongoc_bulkwrite_append_updateone`, `mongoc_bulkwrite_deletemanyopts_destroy`, `mongoc_bulkwrite_deletemanyopts_new`, `mongoc_bulkwrite_deletemanyopts_set_collation`, `mongoc_bulkwrite_deletemanyopts_set_hint`, `mongoc_bulkwrite_deleteoneopts_destroy`, `mongoc_bulkwrite_deleteoneopts_new`, `mongoc_bulkwrite_deleteoneopts_set_collation`, `mongoc_bulkwrite_deleteoneopts_set_hint`, `mongoc_bulkwrite_destroy`, `mongoc_bulkwrite_execute`, `mongoc_bulkwrite_insertoneopts_destroy`, `mongoc_bulkwrite_insertoneopts_new`, `mongoc_bulkwrite_new`, `mongoc_bulkwrite_replaceoneopts_destroy`, `mongoc_bulkwrite_replaceoneopts_new`, `mongoc_bulkwrite_replaceoneopts_set_collation`, `mongoc_bulkwrite_replaceoneopts_set_hint`, `mongoc_bulkwrite_replaceoneopts_set_sort`, `mongoc_bulkwrite_replaceoneopts_set_upsert`, `mongoc_bulkwrite_set_client`, `mongoc_bulkwrite_set_session`, `mongoc_bulkwrite_updatemanyopts_destroy`, `mongoc_bulkwrite_updatemanyopts_new`, `mongoc_bulkwrite_updatemanyopts_set_arrayfilters`, `mongoc_bulkwrite_updatemanyopts_set_collation`, `mongoc_bulkwrite_updatemanyopts_set_hint`, `mongoc_bulkwrite_updatemanyopts_set_upsert`, `mongoc_bulkwrite_updateoneopts_destroy`, `mongoc_bulkwrite_updateoneopts_new`, `mongoc_bulkwrite_updateoneopts_set_arrayfilters`, `mongoc_bulkwrite_updateoneopts_set_collation`, `mongoc_bulkwrite_updateoneopts_set_hint`, `mongoc_bulkwrite_updateoneopts_set_sort`, `mongoc_bulkwrite_updateoneopts_set_upsert`, `mongoc_bulkwriteexception_destroy`, `mongoc_bulkwriteexception_error`, `mongoc_bulkwriteexception_errorreply`, `mongoc_bulkwriteexception_writeconcernerrors`, `mongoc_bulkwriteexception_writeerrors`, `mongoc_bulkwriteopts_destroy`, `mongoc_bulkwriteopts_new`, `mongoc_bulkwriteopts_set_bypassdocumentvalidation`, `mongoc_bulkwriteopts_set_comment`, `mongoc_bulkwriteopts_set_extra`, `mongoc_bulkwriteopts_set_let`, `mongoc_bulkwriteopts_set_ordered`, `mongoc_bulkwriteopts_set_serverid`, `mongoc_bulkwriteopts_set_verboseresults`, `mongoc_bulkwriteopts_set_writeconcern`, `mongoc_bulkwriteresult_deletedcount`, `mongoc_bulkwriteresult_deleteresults`, `mongoc_bulkwriteresult_destroy`, `mongoc_bulkwriteresult_insertedcount`, `mongoc_bulkwriteresult_insertresults`, `mongoc_bulkwriteresult_matchedcount`, `mongoc_bulkwriteresult_modifiedcount`, `mongoc_bulkwriteresult_serverid`, `mongoc_bulkwriteresult_updateresults`, `mongoc_bulkwriteresult_upsertedcount`, `mongoc_client_bulkwrite_new`

### bson.h (64)

`bson_append_array_builder_begin`, `bson_append_array_builder_end`, `bson_append_dbpointer`, `bson_append_iter`, `bson_append_maxkey`, `bson_append_minkey`, `bson_append_regex_w_len`, `bson_append_symbol`, `bson_append_time_t`, `bson_append_timestamp`, `bson_append_timeval`, `bson_append_undefined`, `bson_append_value`, `bson_array_as_canonical_extended_json`, `bson_array_as_legacy_extended_json`, `bson_array_as_relaxed_extended_json`, `bson_array_builder_append_array`, `bson_array_builder_append_array_builder_end`, `bson_array_builder_append_binary`, `bson_array_builder_append_bool`, `bson_array_builder_append_code`, `bson_array_builder_append_date_time`, `bson_array_builder_append_dbpointer`, `bson_array_builder_append_decimal128`, `bson_array_builder_append_document`, `bson_array_builder_append_document_begin`, `bson_array_builder_append_document_end`, `bson_array_builder_append_double`, `bson_array_builder_append_int32`, `bson_array_builder_append_int64`, `bson_array_builder_append_iter`, `bson_array_builder_append_maxkey`, `bson_array_builder_append_minkey`, `bson_array_builder_append_now_utc`, `bson_array_builder_append_null`, `bson_array_builder_append_oid`, `bson_array_builder_append_regex`, `bson_array_builder_append_regex_w_len`, `bson_array_builder_append_symbol`, `bson_array_builder_append_time_t`, `bson_array_builder_append_timestamp`, `bson_array_builder_append_timeval`, `bson_array_builder_append_undefined`, `bson_array_builder_append_utf8`, `bson_array_builder_append_value`, `bson_array_builder_build`, `bson_array_builder_destroy`, `bson_array_builder_new`, `bson_as_json_with_opts`, `bson_as_legacy_extended_json`, `bson_compare`, `bson_copy_to`, `bson_copy_to_excluding`, `bson_copy_to_excluding_noinit`, `bson_copy_to_excluding_noinit_va`, `bson_destroy_with_steal`, `bson_equal`, `bson_get_data`, `bson_new_from_buffer`, `bson_reserve_buffer`, `bson_sized_new`, `bson_steal`, `bson_validate_with_error`, `bson_validate_with_error_and_offset`

### mongoc-client-side-encryption.h (60)

`mongoc_auto_encryption_opts_destroy`, `mongoc_auto_encryption_opts_new`, `mongoc_auto_encryption_opts_set_bypass_auto_encryption`, `mongoc_auto_encryption_opts_set_bypass_query_analysis`, `mongoc_auto_encryption_opts_set_encrypted_fields_map`, `mongoc_auto_encryption_opts_set_extra`, `mongoc_auto_encryption_opts_set_key_expiration`, `mongoc_auto_encryption_opts_set_keyvault_client`, `mongoc_auto_encryption_opts_set_keyvault_client_pool`, `mongoc_auto_encryption_opts_set_keyvault_namespace`, `mongoc_auto_encryption_opts_set_kms_credential_provider_callback`, `mongoc_auto_encryption_opts_set_kms_providers`, `mongoc_auto_encryption_opts_set_schema_map`, `mongoc_auto_encryption_opts_set_tls_opts`, `mongoc_client_encryption_add_key_alt_name`, `mongoc_client_encryption_create_datakey`, `mongoc_client_encryption_create_encrypted_collection`, `mongoc_client_encryption_datakey_opts_destroy`, `mongoc_client_encryption_datakey_opts_new`, `mongoc_client_encryption_datakey_opts_set_keyaltnames`, `mongoc_client_encryption_datakey_opts_set_keymaterial`, `mongoc_client_encryption_datakey_opts_set_masterkey`, `mongoc_client_encryption_decrypt`, `mongoc_client_encryption_delete_key`, `mongoc_client_encryption_destroy`, `mongoc_client_encryption_encrypt`, `mongoc_client_encryption_encrypt_expression`, `mongoc_client_encryption_encrypt_opts_destroy`, `mongoc_client_encryption_encrypt_opts_new`, `mongoc_client_encryption_encrypt_opts_set_algorithm`, `mongoc_client_encryption_encrypt_opts_set_contention_factor`, `mongoc_client_encryption_encrypt_opts_set_keyaltname`, `mongoc_client_encryption_encrypt_opts_set_keyid`, `mongoc_client_encryption_encrypt_opts_set_query_type`, `mongoc_client_encryption_encrypt_opts_set_range_opts`, `mongoc_client_encryption_encrypt_range_opts_destroy`, `mongoc_client_encryption_encrypt_range_opts_new`, `mongoc_client_encryption_encrypt_range_opts_set_max`, `mongoc_client_encryption_encrypt_range_opts_set_min`, `mongoc_client_encryption_encrypt_range_opts_set_precision`, `mongoc_client_encryption_encrypt_range_opts_set_sparsity`, `mongoc_client_encryption_encrypt_range_opts_set_trim_factor`, `mongoc_client_encryption_get_crypt_shared_version`, `mongoc_client_encryption_get_key`, `mongoc_client_encryption_get_key_by_alt_name`, `mongoc_client_encryption_get_keys`, `mongoc_client_encryption_new`, `mongoc_client_encryption_opts_destroy`, `mongoc_client_encryption_opts_new`, `mongoc_client_encryption_opts_set_key_expiration`, `mongoc_client_encryption_opts_set_keyvault_client`, `mongoc_client_encryption_opts_set_keyvault_namespace`, `mongoc_client_encryption_opts_set_kms_credential_provider_callback`, `mongoc_client_encryption_opts_set_kms_providers`, `mongoc_client_encryption_opts_set_tls_opts`, `mongoc_client_encryption_remove_key_alt_name`, `mongoc_client_encryption_rewrap_many_datakey`, `mongoc_client_encryption_rewrap_many_datakey_result_destroy`, `mongoc_client_encryption_rewrap_many_datakey_result_get_bulk_write_result`, `mongoc_client_encryption_rewrap_many_datakey_result_new`

### mongoc-uri.h (53)

`mongoc_uri_copy`, `mongoc_uri_destroy`, `mongoc_uri_get_appname`, `mongoc_uri_get_auth_mechanism`, `mongoc_uri_get_auth_source`, `mongoc_uri_get_compressors`, `mongoc_uri_get_credentials`, `mongoc_uri_get_database`, `mongoc_uri_get_hosts`, `mongoc_uri_get_mechanism_properties`, `mongoc_uri_get_option_as_bool`, `mongoc_uri_get_option_as_int32`, `mongoc_uri_get_option_as_int64`, `mongoc_uri_get_option_as_utf8`, `mongoc_uri_get_options`, `mongoc_uri_get_password`, `mongoc_uri_get_read_concern`, `mongoc_uri_get_read_prefs`, `mongoc_uri_get_read_prefs_t`, `mongoc_uri_get_replica_set`, `mongoc_uri_get_server_monitoring_mode`, `mongoc_uri_get_service`, `mongoc_uri_get_srv_hostname`, `mongoc_uri_get_srv_service_name`, `mongoc_uri_get_ssl`, `mongoc_uri_get_string`, `mongoc_uri_get_tls`, `mongoc_uri_get_username`, `mongoc_uri_get_write_concern`, `mongoc_uri_has_option`, `mongoc_uri_new_for_host_port`, `mongoc_uri_new_with_error`, `mongoc_uri_option_is_bool`, `mongoc_uri_option_is_int32`, `mongoc_uri_option_is_int64`, `mongoc_uri_option_is_utf8`, `mongoc_uri_set_appname`, `mongoc_uri_set_auth_mechanism`, `mongoc_uri_set_auth_source`, `mongoc_uri_set_compressors`, `mongoc_uri_set_database`, `mongoc_uri_set_mechanism_properties`, `mongoc_uri_set_option_as_bool`, `mongoc_uri_set_option_as_int32`, `mongoc_uri_set_option_as_int64`, `mongoc_uri_set_option_as_utf8`, `mongoc_uri_set_password`, `mongoc_uri_set_read_concern`, `mongoc_uri_set_read_prefs_t`, `mongoc_uri_set_server_monitoring_mode`, `mongoc_uri_set_username`, `mongoc_uri_set_write_concern`, `mongoc_uri_unescape`

### mongoc-client.h (41)

`mongoc_client_command`, `mongoc_client_command_simple_with_server_id`, `mongoc_client_command_with_opts`, `mongoc_client_enable_auto_encryption`, `mongoc_client_find_databases`, `mongoc_client_find_databases_with_opts`, `mongoc_client_get_crypt_shared_version`, `mongoc_client_get_database_names`, `mongoc_client_get_database_names_with_opts`, `mongoc_client_get_default_database`, `mongoc_client_get_gridfs`, `mongoc_client_get_handshake_description`, `mongoc_client_get_max_bson_size`, `mongoc_client_get_max_message_size`, `mongoc_client_get_read_concern`, `mongoc_client_get_read_prefs`, `mongoc_client_get_server_description`, `mongoc_client_get_server_descriptions`, `mongoc_client_get_server_status`, `mongoc_client_get_uri`, `mongoc_client_get_write_concern`, `mongoc_client_kill_cursor`, `mongoc_client_new_from_uri_with_error`, `mongoc_client_read_command_with_opts`, `mongoc_client_read_write_command_with_opts`, `mongoc_client_reset`, `mongoc_client_select_server`, `mongoc_client_set_apm_callbacks`, `mongoc_client_set_error_api`, `mongoc_client_set_read_concern`, `mongoc_client_set_read_prefs`, `mongoc_client_set_server_api`, `mongoc_client_set_sockettimeoutms`, `mongoc_client_set_ssl_opts`, `mongoc_client_set_stream_initiator`, `mongoc_client_set_structured_log_opts`, `mongoc_client_set_write_concern`, `mongoc_client_start_session`, `mongoc_client_watch`, `mongoc_client_write_command_with_opts`, `mongoc_server_descriptions_destroy_all`

### mongoc-collection.h (40)

`mongoc_collection_command`, `mongoc_collection_command_with_opts`, `mongoc_collection_copy`, `mongoc_collection_count`, `mongoc_collection_count_documents`, `mongoc_collection_count_with_opts`, `mongoc_collection_create_bulk_operation`, `mongoc_collection_create_index`, `mongoc_collection_create_index_with_opts`, `mongoc_collection_create_indexes_with_opts`, `mongoc_collection_delete`, `mongoc_collection_delete_many`, `mongoc_collection_delete_one`, `mongoc_collection_drop_index`, `mongoc_collection_drop_index_with_opts`, `mongoc_collection_ensure_index`, `mongoc_collection_estimated_document_count`, `mongoc_collection_find_and_modify`, `mongoc_collection_find_and_modify_with_opts`, `mongoc_collection_find_indexes`, `mongoc_collection_get_last_error`, `mongoc_collection_get_name`, `mongoc_collection_get_read_concern`, `mongoc_collection_get_read_prefs`, `mongoc_collection_get_write_concern`, `mongoc_collection_insert_bulk`, `mongoc_collection_read_command_with_opts`, `mongoc_collection_read_write_command_with_opts`, `mongoc_collection_rename`, `mongoc_collection_rename_with_opts`, `mongoc_collection_replace_one`, `mongoc_collection_save`, `mongoc_collection_set_read_concern`, `mongoc_collection_set_read_prefs`, `mongoc_collection_set_write_concern`, `mongoc_collection_stats`, `mongoc_collection_validate`, `mongoc_collection_watch`, `mongoc_index_model_destroy`, `mongoc_index_model_new`

### mongoc-client-session.h (38)

`mongoc_client_session_abort_transaction`, `mongoc_client_session_advance_cluster_time`, `mongoc_client_session_advance_operation_time`, `mongoc_client_session_append`, `mongoc_client_session_commit_transaction`, `mongoc_client_session_destroy`, `mongoc_client_session_get_client`, `mongoc_client_session_get_cluster_time`, `mongoc_client_session_get_dirty`, `mongoc_client_session_get_lsid`, `mongoc_client_session_get_operation_time`, `mongoc_client_session_get_opts`, `mongoc_client_session_get_server_id`, `mongoc_client_session_get_transaction_state`, `mongoc_client_session_in_transaction`, `mongoc_client_session_start_transaction`, `mongoc_client_session_with_transaction`, `mongoc_session_opts_clone`, `mongoc_session_opts_destroy`, `mongoc_session_opts_get_causal_consistency`, `mongoc_session_opts_get_default_transaction_opts`, `mongoc_session_opts_get_snapshot`, `mongoc_session_opts_get_transaction_opts`, `mongoc_session_opts_new`, `mongoc_session_opts_set_causal_consistency`, `mongoc_session_opts_set_default_transaction_opts`, `mongoc_session_opts_set_snapshot`, `mongoc_transaction_opts_clone`, `mongoc_transaction_opts_destroy`, `mongoc_transaction_opts_get_max_commit_time_ms`, `mongoc_transaction_opts_get_read_concern`, `mongoc_transaction_opts_get_read_prefs`, `mongoc_transaction_opts_get_write_concern`, `mongoc_transaction_opts_new`, `mongoc_transaction_opts_set_max_commit_time_ms`, `mongoc_transaction_opts_set_read_concern`, `mongoc_transaction_opts_set_read_prefs`, `mongoc_transaction_opts_set_write_concern`

### bson-iter.h (36)

`bson_iter_bool_unsafe`, `bson_iter_code_unsafe`, `bson_iter_dbpointer`, `bson_iter_decimal128_unsafe`, `bson_iter_double_unsafe`, `bson_iter_dup_utf8`, `bson_iter_find_w_len`, `bson_iter_init_find_w_len`, `bson_iter_init_from_data`, `bson_iter_init_from_data_at_offset`, `bson_iter_int32_unsafe`, `bson_iter_int64_unsafe`, `bson_iter_key_len`, `bson_iter_key_unsafe`, `bson_iter_offset`, `bson_iter_oid_unsafe`, `bson_iter_overwrite_bool`, `bson_iter_overwrite_date_time`, `bson_iter_overwrite_decimal128`, `bson_iter_overwrite_double`, `bson_iter_overwrite_int32`, `bson_iter_overwrite_int64`, `bson_iter_overwrite_oid`, `bson_iter_overwrite_timestamp`, `bson_iter_recurse`, `bson_iter_symbol`, `bson_iter_time_t`, `bson_iter_time_t_unsafe`, `bson_iter_timestamp`, `bson_iter_timeval`, `bson_iter_timeval_unsafe`, `bson_iter_type_unsafe`, `bson_iter_utf8_len_unsafe`, `bson_iter_utf8_unsafe`, `bson_iter_value`, `bson_iter_visit_all`

### bson-atomic.h (27)

`bson_atomic_int32_compare_exchange_strong`, `bson_atomic_int32_compare_exchange_weak`, `bson_atomic_int32_exchange`, `bson_atomic_int32_fetch`, `bson_atomic_int32_fetch_add`, `bson_atomic_int32_fetch_sub`, `bson_atomic_int64_add`, `bson_atomic_int64_compare_exchange_strong`, `bson_atomic_int64_compare_exchange_weak`, `bson_atomic_int64_exchange`, `bson_atomic_int64_fetch_add`, `bson_atomic_int64_fetch_sub`, `bson_atomic_int_add`, `bson_atomic_int_compare_exchange_strong`, `bson_atomic_int_compare_exchange_weak`, `bson_atomic_int_exchange`, `bson_atomic_int_fetch`, `bson_atomic_int_fetch_add`, `bson_atomic_int_fetch_sub`, `bson_atomic_ptr_compare_exchange_strong`, `bson_atomic_ptr_compare_exchange_weak`, `bson_atomic_ptr_exchange`, `bson_atomic_ptr_fetch`, `bson_atomic_thread_fence`, `bson_memory_barrier`, `bson_sync_synchronize`, `bson_thrd_yield`

### mongoc-bulk-operation.h (26)

`mongoc_bulk_operation_delete`, `mongoc_bulk_operation_delete_one`, `mongoc_bulk_operation_get_hint`, `mongoc_bulk_operation_get_server_id`, `mongoc_bulk_operation_get_write_concern`, `mongoc_bulk_operation_new`, `mongoc_bulk_operation_remove`, `mongoc_bulk_operation_remove_many_with_opts`, `mongoc_bulk_operation_remove_one`, `mongoc_bulk_operation_remove_one_with_opts`, `mongoc_bulk_operation_replace_one`, `mongoc_bulk_operation_replace_one_with_opts`, `mongoc_bulk_operation_set_bypass_document_validation`, `mongoc_bulk_operation_set_client`, `mongoc_bulk_operation_set_client_session`, `mongoc_bulk_operation_set_collection`, `mongoc_bulk_operation_set_comment`, `mongoc_bulk_operation_set_database`, `mongoc_bulk_operation_set_hint`, `mongoc_bulk_operation_set_let`, `mongoc_bulk_operation_set_server_id`, `mongoc_bulk_operation_set_write_concern`, `mongoc_bulk_operation_update`, `mongoc_bulk_operation_update_many_with_opts`, `mongoc_bulk_operation_update_one`, `mongoc_bulk_operation_update_one_with_opts`

### mongoc-database.h (24)

`mongoc_database_add_user`, `mongoc_database_aggregate`, `mongoc_database_command`, `mongoc_database_command_simple`, `mongoc_database_command_with_opts`, `mongoc_database_copy`, `mongoc_database_drop`, `mongoc_database_drop_with_opts`, `mongoc_database_find_collections`, `mongoc_database_find_collections_with_opts`, `mongoc_database_get_collection_names`, `mongoc_database_get_name`, `mongoc_database_get_read_concern`, `mongoc_database_get_read_prefs`, `mongoc_database_get_write_concern`, `mongoc_database_has_collection`, `mongoc_database_read_command_with_opts`, `mongoc_database_read_write_command_with_opts`, `mongoc_database_remove_all_users`, `mongoc_database_remove_user`, `mongoc_database_set_read_concern`, `mongoc_database_set_read_prefs`, `mongoc_database_set_write_concern`, `mongoc_database_watch`

### mongoc-cursor.h (20)

`mongoc_cursor_clone`, `mongoc_cursor_current`, `mongoc_cursor_destroy`, `mongoc_cursor_error_document`, `mongoc_cursor_get_batch_size`, `mongoc_cursor_get_hint`, `mongoc_cursor_get_host`, `mongoc_cursor_get_id`, `mongoc_cursor_get_limit`, `mongoc_cursor_get_max_await_time_ms`, `mongoc_cursor_get_server_id`, `mongoc_cursor_is_alive`, `mongoc_cursor_more`, `mongoc_cursor_new_from_command_reply`, `mongoc_cursor_new_from_command_reply_with_opts`, `mongoc_cursor_set_batch_size`, `mongoc_cursor_set_hint`, `mongoc_cursor_set_limit`, `mongoc_cursor_set_max_await_time_ms`, `mongoc_cursor_set_server_id`

### bson-string.h (19)

`bson_ascii_strtoll`, `bson_isspace`, `bson_snprintf`, `bson_strcasecmp`, `bson_strdup`, `bson_strdup_printf`, `bson_strdupv_printf`, `bson_strfreev`, `bson_string_append`, `bson_string_append_c`, `bson_string_append_printf`, `bson_string_append_unichar`, `bson_string_free`, `bson_string_new`, `bson_string_truncate`, `bson_strncpy`, `bson_strndup`, `bson_strnlen`, `bson_vsnprintf`

### mongoc-structured-log.h (18)

`mongoc_structured_log_entry_get_component`, `mongoc_structured_log_entry_get_level`, `mongoc_structured_log_entry_get_message_string`, `mongoc_structured_log_entry_message_as_bson`, `mongoc_structured_log_get_component_name`, `mongoc_structured_log_get_level_name`, `mongoc_structured_log_get_named_component`, `mongoc_structured_log_get_named_level`, `mongoc_structured_log_opts_destroy`, `mongoc_structured_log_opts_get_max_document_length`, `mongoc_structured_log_opts_get_max_level_for_component`, `mongoc_structured_log_opts_new`, `mongoc_structured_log_opts_set_handler`, `mongoc_structured_log_opts_set_max_document_length`, `mongoc_structured_log_opts_set_max_document_length_from_env`, `mongoc_structured_log_opts_set_max_level_for_all_components`, `mongoc_structured_log_opts_set_max_level_for_component`, `mongoc_structured_log_opts_set_max_levels_from_env`

### mongoc-socket.h (17)

`mongoc_socket_accept`, `mongoc_socket_bind`, `mongoc_socket_check_closed`, `mongoc_socket_close`, `mongoc_socket_connect`, `mongoc_socket_destroy`, `mongoc_socket_errno`, `mongoc_socket_getnameinfo`, `mongoc_socket_getsockname`, `mongoc_socket_inet_ntop`, `mongoc_socket_listen`, `mongoc_socket_new`, `mongoc_socket_poll`, `mongoc_socket_recv`, `mongoc_socket_send`, `mongoc_socket_sendv`, `mongoc_socket_setsockopt`

### mongoc-find-and-modify.h (16)

`mongoc_find_and_modify_opts_append`, `mongoc_find_and_modify_opts_destroy`, `mongoc_find_and_modify_opts_get_bypass_document_validation`, `mongoc_find_and_modify_opts_get_extra`, `mongoc_find_and_modify_opts_get_fields`, `mongoc_find_and_modify_opts_get_flags`, `mongoc_find_and_modify_opts_get_max_time_ms`, `mongoc_find_and_modify_opts_get_sort`, `mongoc_find_and_modify_opts_get_update`, `mongoc_find_and_modify_opts_new`, `mongoc_find_and_modify_opts_set_bypass_document_validation`, `mongoc_find_and_modify_opts_set_fields`, `mongoc_find_and_modify_opts_set_flags`, `mongoc_find_and_modify_opts_set_max_time_ms`, `mongoc_find_and_modify_opts_set_sort`, `mongoc_find_and_modify_opts_set_update`

### mongoc-client-pool.h (15)

`mongoc_client_pool_destroy`, `mongoc_client_pool_enable_auto_encryption`, `mongoc_client_pool_max_size`, `mongoc_client_pool_min_size`, `mongoc_client_pool_new`, `mongoc_client_pool_new_with_error`, `mongoc_client_pool_pop`, `mongoc_client_pool_push`, `mongoc_client_pool_set_apm_callbacks`, `mongoc_client_pool_set_appname`, `mongoc_client_pool_set_error_api`, `mongoc_client_pool_set_server_api`, `mongoc_client_pool_set_ssl_opts`, `mongoc_client_pool_set_structured_log_opts`, `mongoc_client_pool_try_pop`

### mongoc-stream.h (15)

`mongoc_stream_check_closed`, `mongoc_stream_close`, `mongoc_stream_destroy`, `mongoc_stream_failed`, `mongoc_stream_flush`, `mongoc_stream_get_base_stream`, `mongoc_stream_get_tls_stream`, `mongoc_stream_poll`, `mongoc_stream_read`, `mongoc_stream_readv`, `mongoc_stream_setsockopt`, `mongoc_stream_should_retry`, `mongoc_stream_timed_out`, `mongoc_stream_write`, `mongoc_stream_writev`

### mongoc-gridfs-bucket.h (12)

`mongoc_gridfs_bucket_abort_upload`, `mongoc_gridfs_bucket_delete_by_id`, `mongoc_gridfs_bucket_destroy`, `mongoc_gridfs_bucket_download_to_stream`, `mongoc_gridfs_bucket_find`, `mongoc_gridfs_bucket_new`, `mongoc_gridfs_bucket_open_download_stream`, `mongoc_gridfs_bucket_open_upload_stream`, `mongoc_gridfs_bucket_open_upload_stream_with_id`, `mongoc_gridfs_bucket_stream_error`, `mongoc_gridfs_bucket_upload_from_stream`, `mongoc_gridfs_bucket_upload_from_stream_with_id`

### mongoc-gridfs.h (12)

`mongoc_gridfs_create_file`, `mongoc_gridfs_create_file_from_stream`, `mongoc_gridfs_destroy`, `mongoc_gridfs_drop`, `mongoc_gridfs_find`, `mongoc_gridfs_find_one`, `mongoc_gridfs_find_one_by_filename`, `mongoc_gridfs_find_one_with_opts`, `mongoc_gridfs_find_with_opts`, `mongoc_gridfs_get_chunks`, `mongoc_gridfs_get_files`, `mongoc_gridfs_remove_by_filename`

### mongoc-gridfs-file.h (12)

`mongoc_gridfs_file_destroy`, `mongoc_gridfs_file_error`, `mongoc_gridfs_file_get_chunk_size`, `mongoc_gridfs_file_get_length`, `mongoc_gridfs_file_get_upload_date`, `mongoc_gridfs_file_readv`, `mongoc_gridfs_file_remove`, `mongoc_gridfs_file_save`, `mongoc_gridfs_file_seek`, `mongoc_gridfs_file_set_id`, `mongoc_gridfs_file_tell`, `mongoc_gridfs_file_writev`

### bson-memory.h (10)

`bson_aligned_alloc`, `bson_aligned_alloc0`, `bson_free`, `bson_malloc`, `bson_malloc0`, `bson_mem_restore_vtable`, `bson_mem_set_vtable`, `bson_realloc`, `bson_realloc_ctx`, `bson_zero_free`

### bson-json.h (10)

`bson_json_data_reader_ingest`, `bson_json_data_reader_new`, `bson_json_opts_destroy`, `bson_json_opts_new`, `bson_json_opts_set_outermost_array`, `bson_json_reader_destroy`, `bson_json_reader_new`, `bson_json_reader_new_from_fd`, `bson_json_reader_new_from_file`, `bson_json_reader_read`

### bson-reader.h (10)

`bson_reader_destroy`, `bson_reader_new_from_data`, `bson_reader_new_from_fd`, `bson_reader_new_from_file`, `bson_reader_new_from_handle`, `bson_reader_read`, `bson_reader_reset`, `bson_reader_set_destroy_func`, `bson_reader_set_read_func`, `bson_reader_tell`

### mongoc-server-api.h (10)

`mongoc_server_api_copy`, `mongoc_server_api_deprecation_errors`, `mongoc_server_api_destroy`, `mongoc_server_api_get_deprecation_errors`, `mongoc_server_api_get_strict`, `mongoc_server_api_get_version`, `mongoc_server_api_new`, `mongoc_server_api_strict`, `mongoc_server_api_version_from_string`, `mongoc_server_api_version_to_string`

### mongoc-server-description.h (10)

`mongoc_server_description_compressor_id`, `mongoc_server_description_destroy`, `mongoc_server_description_hello_response`, `mongoc_server_description_host`, `mongoc_server_description_id`, `mongoc_server_description_ismaster`, `mongoc_server_description_last_update_time`, `mongoc_server_description_new_copy`, `mongoc_server_description_round_trip_time`, `mongoc_server_description_type`

### bson-oid.h (8)

`bson_oid_compare_unsafe`, `bson_oid_copy_unsafe`, `bson_oid_equal_unsafe`, `bson_oid_get_time_t_unsafe`, `bson_oid_hash_unsafe`, `bson_oid_init_from_string_unsafe`, `bson_oid_init_sequence`, `bson_oid_parse_hex_char`

### mongoc-read-concern.h (7)

`mongoc_read_concern_append`, `mongoc_read_concern_copy`, `mongoc_read_concern_destroy`, `mongoc_read_concern_get_level`, `mongoc_read_concern_is_default`, `mongoc_read_concern_new`, `mongoc_read_concern_set_level`

### bson-writer.h (6)

`bson_writer_begin`, `bson_writer_destroy`, `bson_writer_end`, `bson_writer_get_length`, `bson_writer_new`, `bson_writer_rollback`

### mongoc-index.h (6)

`mongoc_index_opt_geo_get_default`, `mongoc_index_opt_geo_init`, `mongoc_index_opt_get_default`, `mongoc_index_opt_init`, `mongoc_index_opt_wt_get_default`, `mongoc_index_opt_wt_init`

### mongoc-log.h (6)

`mongoc_log`, `mongoc_log_default_handler`, `mongoc_log_level_str`, `mongoc_log_set_handler`, `mongoc_log_trace_disable`, `mongoc_log_trace_enable`

### mongoc-stream-tls.h (6)

`mongoc_stream_tls_check_cert`, `mongoc_stream_tls_do_handshake`, `mongoc_stream_tls_handshake`, `mongoc_stream_tls_handshake_block`, `mongoc_stream_tls_new`, `mongoc_stream_tls_new_with_hostname`

### mongoc-topology-description.h (6)

`mongoc_topology_description_destroy`, `mongoc_topology_description_get_servers`, `mongoc_topology_description_has_readable_server`, `mongoc_topology_description_has_writable_server`, `mongoc_topology_description_new_copy`, `mongoc_topology_description_type`

### bson-utf8.h (5)

`bson_utf8_escape_for_json`, `bson_utf8_from_unichar`, `bson_utf8_get_char`, `bson_utf8_next_char`, `bson_utf8_validate`

### mongoc-optional.h (5)

`mongoc_optional_copy`, `mongoc_optional_init`, `mongoc_optional_is_set`, `mongoc_optional_set_value`, `mongoc_optional_value`

### mongoc-write-concern.h (5)

`mongoc_write_concern_destroy`, `mongoc_write_concern_get_fsync`, `mongoc_write_concern_get_wtimeout_int64`, `mongoc_write_concern_set_fsync`, `mongoc_write_concern_set_wtimeout_int64`

### mongoc-change-stream.h (4)

`mongoc_change_stream_destroy`, `mongoc_change_stream_error_document`, `mongoc_change_stream_get_resume_token`, `mongoc_change_stream_next`

### bson-md5.h (3)

`bson_md5_append`, `bson_md5_finish`, `bson_md5_init`

### mongoc-gridfs-file-list.h (3)

`mongoc_gridfs_file_list_destroy`, `mongoc_gridfs_file_list_error`, `mongoc_gridfs_file_list_next`

### mongoc-matcher.h (3)

`mongoc_matcher_destroy`, `mongoc_matcher_match`, `mongoc_matcher_new`

### mongoc-rand.h (3)

`mongoc_rand_add`, `mongoc_rand_seed`, `mongoc_rand_status`

### mongoc-read-prefs.h (3)

`mongoc_read_prefs_destroy`, `mongoc_read_prefs_get_hedge`, `mongoc_read_prefs_set_hedge`

### mongoc-stream-file.h (3)

`mongoc_stream_file_get_fd`, `mongoc_stream_file_new`, `mongoc_stream_file_new_for_path`

### bcon.h (2)

`bson_bcon_magic`, `bson_bcone_magic`

### bson-clock.h (2)

`bson_get_monotonic_time`, `bson_gettimeofday`

### bson-types.h (2)

`bson_is_power_of_two`, `bson_next_power_of_two`

### bson-error.h (2)

`bson_set_error`, `bson_strerror_r`

### bson-value.h (2)

`bson_value_copy`, `bson_value_destroy`

### mongoc-sleep.h (2)

`mongoc_client_set_usleep_impl`, `mongoc_usleep_default_impl`

### mongoc-stream-socket.h (2)

`mongoc_stream_socket_get_socket`, `mongoc_stream_socket_new`

### bson-decimal128.h (1)

`bson_decimal128_from_string_w_len`

### bson-keys.h (1)

`bson_uint32_to_string`

### mongoc-error.h (1)

`mongoc_error_has_label`

### mongoc-handshake.h (1)

`mongoc_handshake_data_append`

### mongoc-ssl.h (1)

`mongoc_ssl_opt_get_default`

### mongoc-stream-buffered.h (1)

`mongoc_stream_buffered_new`

### mongoc-stream-gridfs.h (1)

`mongoc_stream_gridfs_new`

### mongoc-stream-tls-libressl.h (1)

`mongoc_stream_tls_libressl_new`

### mongoc-stream-tls-openssl.h (1)

`mongoc_stream_tls_openssl_new`

### mongoc-stream-tls-secure-channel.h (1)

`mongoc_stream_tls_secure_channel_new`

### mongoc-stream-tls-secure-transport.h (1)

`mongoc_stream_tls_secure_transport_new`

## Removed in 2.x (project calls these; will not compile against 2.5.5)

- `bson_append_array_begin`
- `bson_array_as_json`
- `bson_as_json`
- `mongoc_collection_find`
## New in 2.x (not in 1.30.12) — 118 total

`bson_append_array_from_vector`, `bson_append_array_from_vector_float32`, `bson_append_array_from_vector_int8`, `bson_append_array_from_vector_packed_bit`, `bson_append_array_unsafe_begin`, `bson_append_binary_uninit`, `bson_append_vector_float32_from_array`, `bson_append_vector_float32_uninit`, `bson_append_vector_int8_from_array`, `bson_append_vector_int8_uninit`, `bson_append_vector_packed_bit_from_array`, `bson_append_vector_packed_bit_uninit`, `bson_array_alloc`, `bson_array_alloc0`, `bson_array_builder_append_array_builder_begin`, `bson_array_builder_append_array_from_vector`, `bson_array_builder_append_vector_elements`, `bson_array_builder_append_vector_float32_elements`, `bson_array_builder_append_vector_int8_elements`, `bson_array_builder_append_vector_packed_bit_elements`, `bson_error_clear`, `bson_iter_binary_equal`, `bson_iter_binary_subtype`, `bson_iter_overwrite_binary`, `bson_vector_binary_view_impl_as_const`, `bson_vector_float32_binary_data_length`, `bson_vector_float32_const_view_from_iter`, `bson_vector_float32_const_view_init`, `bson_vector_float32_const_view_length`, `bson_vector_float32_const_view_read`, `bson_vector_float32_view_as_const`, `bson_vector_float32_view_from_iter`, `bson_vector_float32_view_init`, `bson_vector_float32_view_length`, `bson_vector_float32_view_read`, `bson_vector_float32_view_write`, `bson_vector_int8_binary_data_length`, `bson_vector_int8_const_view_from_iter`, `bson_vector_int8_const_view_init`, `bson_vector_int8_const_view_length`, `bson_vector_int8_const_view_read`, `bson_vector_int8_view_as_const`, `bson_vector_int8_view_from_iter`, `bson_vector_int8_view_init`, `bson_vector_int8_view_length`, `bson_vector_int8_view_pointer`, `bson_vector_int8_view_read`, `bson_vector_int8_view_write`, `bson_vector_packed_bit_binary_data_length`, `bson_vector_packed_bit_const_view_from_iter`, `bson_vector_packed_bit_const_view_init`, `bson_vector_packed_bit_const_view_length`, `bson_vector_packed_bit_const_view_length_bytes`, `bson_vector_packed_bit_const_view_padding`, `bson_vector_packed_bit_const_view_read_packed`, `bson_vector_packed_bit_const_view_unpack_bool`, `bson_vector_packed_bit_view_as_const`, `bson_vector_packed_bit_view_from_iter`, `bson_vector_packed_bit_view_init`, `bson_vector_packed_bit_view_length`, `bson_vector_packed_bit_view_length_bytes`, `bson_vector_packed_bit_view_pack_bool`, `bson_vector_packed_bit_view_padding`, `bson_vector_packed_bit_view_read_packed`, `bson_vector_packed_bit_view_unpack_bool`, `bson_vector_packed_bit_view_write_packed`, `bson_vector_padding_from_header_byte_1`, `mongoc_bulk_operation_get_bypass_document_validation`, `mongoc_bulk_operation_get_collection`, `mongoc_bulk_operation_get_comment`, `mongoc_bulk_operation_get_database`, `mongoc_bulk_operation_get_let`, `mongoc_bulkwrite_check_acknowledged`, `mongoc_bulkwrite_serverid`, `mongoc_client_append_metadata`, `mongoc_client_encryption_encrypt_opts_set_string_opts`, `mongoc_client_encryption_encrypt_string_opts_destroy`, `mongoc_client_encryption_encrypt_string_opts_new`, `mongoc_client_encryption_encrypt_string_opts_set_case_sensitive`, `mongoc_client_encryption_encrypt_string_opts_set_diacritic_sensitive`, `mongoc_client_encryption_encrypt_string_opts_set_prefix`, `mongoc_client_encryption_encrypt_string_opts_set_substring`, `mongoc_client_encryption_encrypt_string_opts_set_suffix`, `mongoc_client_encryption_encrypt_string_prefix_opts_destroy`, `mongoc_client_encryption_encrypt_string_prefix_opts_new`, `mongoc_client_encryption_encrypt_string_prefix_opts_set_str_max_query_length`, `mongoc_client_encryption_encrypt_string_prefix_opts_set_str_min_query_length`, `mongoc_client_encryption_encrypt_string_substring_opts_destroy`, `mongoc_client_encryption_encrypt_string_substring_opts_new`, `mongoc_client_encryption_encrypt_string_substring_opts_set_str_max_length`, `mongoc_client_encryption_encrypt_string_substring_opts_set_str_max_query_length`, `mongoc_client_encryption_encrypt_string_substring_opts_set_str_min_query_length`, `mongoc_client_encryption_encrypt_string_suffix_opts_destroy`, `mongoc_client_encryption_encrypt_string_suffix_opts_new`, `mongoc_client_encryption_encrypt_string_suffix_opts_set_str_max_query_length`, `mongoc_client_encryption_encrypt_string_suffix_opts_set_str_min_query_length`, `mongoc_client_pool_append_metadata`, `mongoc_client_pool_set_oidc_callback`, `mongoc_client_session_get_snapshot_time`, `mongoc_client_set_oidc_callback`, `mongoc_oidc_callback_destroy`, `mongoc_oidc_callback_get_fn`, `mongoc_oidc_callback_get_user_data`, `mongoc_oidc_callback_new`, `mongoc_oidc_callback_new_with_user_data`, `mongoc_oidc_callback_params_cancel_with_timeout`, `mongoc_oidc_callback_params_get_timeout`, `mongoc_oidc_callback_params_get_user_data`, `mongoc_oidc_callback_params_get_username`, `mongoc_oidc_callback_params_get_version`, `mongoc_oidc_callback_set_user_data`, `mongoc_oidc_credential_destroy`, `mongoc_oidc_credential_get_access_token`, `mongoc_oidc_credential_get_expires_in`, `mongoc_oidc_credential_new`, `mongoc_oidc_credential_new_with_expires_in`, `mongoc_session_opts_get_snapshot_time`, `mongoc_session_opts_set_snapshot_time`

## Deprecated in 1.30.12 (still wrapped by project)

- `bson_as_json`
- `bson_init`
- `mongoc_collection_find`
## Project-only names (not upstream libbson/mongoc symbols)

- `bson_new`
- `hb_bson_as_hash`
- `hb_bson_as_json`
- `hb_bson_set_return_json_type`
- `hb_bson_version`
- `hb_dttounix`
- `hb_mongoc_set_return_bson_value_type`
- `hb_numtype`
- `hb_unixtot`