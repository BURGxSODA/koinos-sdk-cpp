#include <koinos/system/system_calls.hpp>

#include <koinos/protocol/system_call_ids.h>
#include <koinos/buffer.hpp>
#include <koinos/common.h>

#include <array>
#include <string>

extern "C" void invoke_system_call( uint32_t sid, char* ret_ptr, uint32_t ret_len, char* arg_ptr, uint32_t arg_len );

namespace koinos::system {

namespace detail {
   static std::array< uint8_t, max_buffer_size > return_buffer;
   static std::array< uint8_t, max_buffer_size > argument_buffer;
}

void print( const std::string& s )
{
   koinos::chain::prints_args< detail::max_argument_size > args;
   args.mutable_message() = s.c_str();

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::prints ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

bool verify_block_signature( const std::string& digest, const std::string& active_data, const std::string& sig )
{
   koinos::chain::verify_block_signature_args<
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_active_data_size > args;

   args.mutable_digest().set( reinterpret_cast< const uint8_t* >( digest.data() ), digest.size() );
   args.mutable_active_blob().set( reinterpret_cast< const uint8_t* >( active_data.data() ), active_data.size() );
   args.mutable_signature_data().set( reinterpret_cast< const uint8_t* >( sig.data() ), sig.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::verify_block_signature ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::verify_block_signature_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

void apply_block( const block& b, bool check_passive_data, bool check_block_signature, bool check_transaction_signatures )
{
   koinos::chain::apply_block_args<
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_active_data_size,
      detail::max_passive_data_size,
      detail::max_hash_size,
      detail::max_transaction_length,
      detail::max_hash_size,
      detail::max_active_data_size,
      detail::max_passive_data_size,
      detail::max_hash_size > args;
   args.mutable_block() = b;
   args.set_check_passive_data( check_passive_data );
   args.set_check_block_signature( check_block_signature );
   args.set_check_transaction_signature( check_transaction_signatures );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::apply_block ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

void apply_transaction( const transaction& t )
{
   koinos::chain::apply_transaction_args<
      detail::max_hash_size,
      detail::max_active_data_size,
      detail::max_passive_data_size,
      detail::max_hash_size > args;
   args.mutable_transaction() = t;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::apply_transaction ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

void apply_upload_contract_operation( const upload_contract_operation& o )
{
   koinos::chain::apply_upload_contract_operation_args<
      detail::max_hash_size,
      detail::max_contract_size > args;
   args.mutable_op() = o;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::apply_upload_contract_operation ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

void apply_call_contract_operation( const call_contract_operation& o )
{
   koinos::chain::apply_call_contract_operation_args<
      detail::max_hash_size,
      detail::max_argument_size > args;
   args.mutable_op() = o;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::apply_call_contract_operation ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

void apply_set_system_call_operation( const set_system_call_operation& o )
{
   koinos::chain::apply_set_system_call_operation_args< detail::max_argument_size > args;
   args.mutable_op() = o;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::apply_call_contract_operation ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

bool put_object( const std::string& space, const std::string& key, const std::string& obj )
{
   koinos::chain::put_object_args< detail::max_space_size, detail::max_key_size, detail::max_argument_size > args;
   args.mutable_key().set( reinterpret_cast< const uint8_t* >( space.data() ), space.size() );
   args.mutable_space().set( reinterpret_cast< const uint8_t* >( key.data() ), key.size() );
   args.mutable_obj().set( reinterpret_cast< const uint8_t* >( obj.data() ), obj.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::put_object ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::put_object_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

std::string get_object( const std::string& space, const std::string& key, int32_t object_size_hint )
{
   koinos::chain::get_object_args< detail::max_space_size, detail::max_key_size > args;
   args.mutable_space().set( reinterpret_cast< const uint8_t* >( space.data() ), space.size() );
   args.mutable_key().set( reinterpret_cast< const uint8_t* >( key.data() ), key.size() );
   args.mutable_object_size_hint() = object_size_hint;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::put_object ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_object_return< detail::max_argument_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

std::string get_next_object( const std::string& space, const std::string& key, int32_t object_size_hint )
{
   koinos::chain::get_next_object_args< detail::max_space_size, detail::max_key_size > args;
   args.mutable_space().set( reinterpret_cast< const uint8_t* >( space.data() ), space.size() );
   args.mutable_key().set( reinterpret_cast< const uint8_t* >( key.data() ), key.size() );
   args.mutable_object_size_hint() = object_size_hint;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_next_object ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_next_object_return< detail::max_argument_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

std::string get_prev_object( const std::string& space, const std::string& key, int32_t object_size_hint )
{
   koinos::chain::get_prev_object_args< detail::max_space_size, detail::max_key_size > args;
   args.mutable_space().set( reinterpret_cast< const uint8_t* >( space.data() ), space.size() );
   args.mutable_key().set( reinterpret_cast< const uint8_t* >( key.data() ), key.size() );
   args.mutable_object_size_hint() = object_size_hint;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_prev_object ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_prev_object_return< detail::max_argument_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

std::string call_contract( const std::string& contract_id, uint32_t entry_point, const std::string& contract_args )
{
   koinos::chain::call_contract_args< detail::max_hash_size, detail::max_argument_size > args;
   args.mutable_contract_id().set( reinterpret_cast< const uint8_t* >( contract_id.data() ), contract_id.size() );
   args.mutable_entry_point() = entry_point;
   args.mutable_args().set( reinterpret_cast< const uint8_t* >( contract_args.data() ), contract_args.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::call_contract ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::call_contract_return< detail::max_argument_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

uint32_t get_entry_point()
{
   koinos::chain::get_entry_point_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_entry_point ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_entry_point_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

uint32_t get_contract_args_size()
{
   koinos::chain::get_contract_args_size_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_contract_args ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_contract_args_size_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

std::string get_contract_args()
{
   koinos::chain::get_contract_args_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_contract_args ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_contract_args_return< detail::max_argument_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

void set_contract_return_bytes( const std::string& ret )
{
   koinos::chain::set_contract_return_args< detail::max_argument_size > args;
   args.mutable_value().set( reinterpret_cast< const uint8_t* >( ret.data() ), ret.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::set_contract_return ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

void exit_contract( uint8_t exit_code )
{
   koinos::chain::exit_contract_args args;
   args.mutable_exit_code() = exit_code;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::exit_contract ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

head_info get_head_info()
{
   koinos::chain::get_head_info_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_head_info ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_head_info_return< detail::max_hash_size, detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

std::string hash( uint64_t code, const std::string& obj, uint64_t size )
{
   koinos::chain::hash_args< detail::max_argument_size > args;
   args.mutable_code() = code;
   args.mutable_obj().set( reinterpret_cast< const uint8_t* >( obj.data() ), obj.size() );
   args.mutable_size() = size;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::hash ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::hash_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

std::string recover_public_key( const std::string& signature_data, const std::string& digest )
{
   koinos::chain::recover_public_key_args< detail::max_hash_size, detail::max_hash_size > args;
   args.mutable_digest().set( reinterpret_cast< const uint8_t* >( digest.data() ), digest.size() );
   args.mutable_signature_data().set( reinterpret_cast< const uint8_t* >( signature_data.data() ), signature_data.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::recover_public_key ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::recover_public_key_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

bool verify_merkle_root( const std::string& root, const std::vector< std::string >& hashes )
{
   koinos::chain::verify_block_signature_args<
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_hash_size,
      detail::max_active_data_size > args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::verify_block_signature ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::verify_block_signature_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}


std::string get_transaction_payer( const transaction& trx )
{
   koinos::chain::get_transaction_payer_args<
      detail::max_hash_size,
      detail::max_active_data_size,
      detail::max_passive_data_size,
      detail::max_hash_size > args;
   args.mutable_transaction() = trx;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_transaction_payer ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_transaction_payer_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ), ret.get_value().get_length() );
}

uint64_t get_max_account_resources( const std::string& account )
{
   koinos::chain::get_max_account_resources_args< detail::max_hash_size > args;
   args.mutable_account().set( reinterpret_cast< const uint8_t* >( account.data() ), account.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_max_account_resources ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_max_account_resources_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

uint64_t get_transaction_resource_limit( const transaction& trx )
{
   koinos::chain::get_transaction_resource_limit_args<
      detail::max_hash_size,
      detail::max_active_data_size,
      detail::max_passive_data_size,
      detail::max_hash_size > args;
   args.mutable_transaction() = trx;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_transaction_resource_limit ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_transaction_resource_limit_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

uint64_t get_last_irreversible_block()
{
   koinos::chain::get_last_irreversible_block_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_last_irreversible_block ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_last_irreversible_block_return ret;
   ret.deserialize( rdbuf );

   return ret.get_value();
}

std::pair< std::string, koinos::chain::privilege > get_caller()
{
   koinos::chain::get_caller_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_caller ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_caller_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::make_pair( std::string( reinterpret_cast< const char* >( ret.caller() ) ), ret.get_caller_privilege() );
}

void require_authority( const std::string& account )
{
   koinos::chain::require_authority_args< detail::max_hash_size > args;
   args.mutable_account().set( reinterpret_cast< const uint8_t* >( account.data() ), account.size() );

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::require_authority ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );
}

std::string get_transaction_signature()
{
   koinos::chain::get_transaction_signature_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_transaction_signature ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_transaction_signature_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ) );
}

std::string get_contract_id()
{
   koinos::chain::get_contract_id_args args;

   koinos::write_buffer buffer( detail::argument_buffer.data(), detail::argument_buffer.size() );
   args.serialize( buffer );

   invoke_system_call(
      std::underlying_type_t< koinos::protocol::system_call_id >( koinos::protocol::system_call_id::get_contract_id ),
      reinterpret_cast< char* >( detail::return_buffer.data() ),
      std::size( detail::return_buffer ),
      reinterpret_cast< char* >( detail::argument_buffer.data() ),
      std::size( detail::argument_buffer )
   );

   koinos::read_buffer rdbuf( detail::return_buffer.data(), detail::return_buffer.size() );

   koinos::chain::get_contract_id_return< detail::max_hash_size > ret;
   ret.deserialize( rdbuf );

   return std::string( reinterpret_cast< const char* >( ret.get_value().get_const() ) );
}

} // koinos::system
