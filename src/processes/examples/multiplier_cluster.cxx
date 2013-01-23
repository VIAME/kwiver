/*ckwg +5
 * Copyright 2012-2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "multiplier_cluster.h"

#include <vistk/pipeline/process_registry.h>

/**
 * \file multiplier_cluster.cxx
 *
 * \brief Implementation of the multiplier cluster.
 */

namespace vistk
{

class multiplier_cluster::priv
{
  public:
    priv();
    ~priv();

    static name_t const name_const;
    static name_t const name_multiplication;

    static type_t const type_const;
    static type_t const type_multiplication;

    static config::key_t const config_factor;
    static port_t const port_factor;
    static port_t const port_output;
};

process::name_t const multiplier_cluster::priv::name_const = name_t("const");
process::name_t const multiplier_cluster::priv::name_multiplication = name_t("multiplication");
process::type_t const multiplier_cluster::priv::type_const = type_t("const_number");
process::type_t const multiplier_cluster::priv::type_multiplication = type_t("multiplication");
config::key_t const multiplier_cluster::priv::config_factor = config::key_t("factor");
process::port_t const multiplier_cluster::priv::port_factor = port_t("factor");
process::port_t const multiplier_cluster::priv::port_output = port_t("product");

multiplier_cluster
::multiplier_cluster(config_t const& config)
  : process_cluster(config)
  , d(new priv)
{
  declare_configuration_key(
    priv::config_factor,
    config::value_t(),
    config::description_t("The value to start counting at."));

  port_flags_t required;

  required.insert(flag_required);

  declare_input_port(
    priv::port_factor,
    "integer",
    required,
    port_description_t("The factor to multiply."));
  declare_output_port(
    priv::port_output,
    "integer",
    required,
    port_description_t("Where the product will be available."));

  config::key_t const const_key = config::key_t("value");

  map_config(priv::config_factor, priv::name_const, const_key);

  add_process(priv::name_const, priv::type_const);
  add_process(priv::name_multiplication, priv::type_multiplication);

  port_t const mult_input_port1 = port_t("factor1");
  port_t const mult_output_port = port_t("product");

  map_input(priv::port_factor, priv::name_multiplication, mult_input_port1);
  map_output(priv::port_output, priv::name_multiplication, mult_output_port);

  port_t const const_port = port_t("number");
  port_t const mult_input_port2 = port_t("factor2");

  connect(priv::name_const, const_port,
          priv::name_multiplication, mult_input_port2);
}

multiplier_cluster
::~multiplier_cluster()
{
}

multiplier_cluster::priv
::priv()
{
}

multiplier_cluster::priv
::~priv()
{
}

}
