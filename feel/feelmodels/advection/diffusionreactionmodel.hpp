/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim:fenc=utf-8:ft=cpp:et:sw=4:ts=4:sts=4

 This file is part of the Feel library

 Author(s): Thibaut Metivet <thibaut.metivet@univ-grenoble-alpes.fr>
 Date: 2016-09-20

 Copyright (C) 2016 Université Joseph Fourier (Grenoble I)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3.0 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
/**
 \file diffusionreactionmodel.hpp
 \author Thibaut Metivet <thibaut.metivet@univ-grenoble-alpes.fr>
 \date 2016-09-20
 */

#ifndef FEELPP_ADVECTION_DIFFUSIONREACTIONMODEL_H
#define FEELPP_ADVECTION_DIFFUSIONREACTIONMODEL_H 1


namespace Feel
{
namespace FeelModels
{

template<class SpaceType>
class DiffusionReactionModel 
{
    typedef DiffusionReactionModel<SpaceType> self_type;
public :
    typedef SpaceType space_type;
    typedef boost::shared_ptr<SpaceType> space_ptrtype;
    typedef typename SpaceType::element_type element_type;
    typedef boost::shared_ptr<element_type> element_ptrtype;
    typedef typename space_type::mesh_type mesh_type;
    typedef boost::shared_ptr<mesh_type> mesh_ptrtype;
    
    static std::string defaultMaterialName() { return std::string("FEELPP_DEFAULT_MATERIAL_NAME"); }

    DiffusionReactionModel( std::string prefix )
    {
        M_cstDiffusionCoeff[self_type::defaultMaterialName()] = doption(_name="D",_prefix=prefix);
        M_cstReactionCoeff[self_type::defaultMaterialName()] = doption(_name="R",_prefix=prefix);
    }
    DiffusionReactionModel( DiffusionReactionModel const& app  ) = default;

    void initFromMesh( mesh_ptrtype const& mesh, bool useExtendedDofTable )
    {
        M_space = space_type::New( _mesh=mesh, _worldscomm=std::vector<WorldComm>(1,mesh->worldComm()),
                                   _extended_doftable=std::vector<bool>(1,useExtendedDofTable) );
        M_fieldDiffusionCoeff = this->functionSpace()->elementPtr( cst( this->cstDiffusionCoeff() ) );
        M_fieldReactionCoeff = this->functionSpace()->elementPtr( cst( this->cstReactionCoeff() ) );
    }

    void initFromSpace( space_ptrtype const& space )
    {
        M_space = space;
        M_fieldDiffusionCoeff = this->functionSpace()->elementPtr( cst( this->cstDiffusionCoeff() ) );
        M_fieldReactionCoeff = this->functionSpace()->elementPtr( cst( this->cstReactionCoeff() ) );
    }

    space_ptrtype const& functionSpace() const { return M_space; }

    double cstDiffusionCoeff( std::string const& marker = "" ) const
    {
        std::string markerUsed = ( marker.empty() )? self_type::defaultMaterialName() : marker;
        auto itFindMarker = M_cstDiffusionCoeff.find( markerUsed );
        CHECK( itFindMarker != M_cstDiffusionCoeff.end() ) << "invalid marker not registered " << markerUsed;
        return itFindMarker->second;
    }
    void setCstDiffusionCoeff(double d, std::string const& marker = "" )
    {
        std::string markerUsed = ( marker.empty() )? self_type::defaultMaterialName() : marker;
        M_cstDiffusionCoeff[markerUsed]=d;
        this->updateDiffusionCoeff( cst(d),marker);
    }
    element_type const& fieldDiffusionCoeff() const { return *M_fieldDiffusionCoeff; }
    element_ptrtype const& fieldDiffusionCoeffPtr() const { return M_fieldDiffusionCoeff; }

    double cstReactionCoeff( std::string const& marker = "" ) const
    {
        std::string markerUsed = ( marker.empty() )? self_type::defaultMaterialName() : marker;
        auto itFindMarker = M_cstReactionCoeff.find( markerUsed );
        CHECK( itFindMarker != M_cstReactionCoeff.end() ) << "invalid marker not registered " << markerUsed;
        return itFindMarker->second;
    }
    void setCstReactionCoeff( double d, std::string const& marker = "" )
    {
        std::string markerUsed = ( marker.empty() )? self_type::defaultMaterialName() : marker;
        M_cstReactionCoeff[markerUsed]=d;
        this->updateReactionCoeff(cst(d),marker);
    }
    element_type const& fieldReactionCoeff() const { return *M_fieldReactionCoeff; }
    element_ptrtype const& fieldReactionCoeffPtr() const { return M_fieldReactionCoeff; }


    template < typename ExprT >
    void updateDiffusionCoeff( vf::Expr<ExprT> const& __expr, std::string const& marker = "" )
    {
        if ( !M_fieldDiffusionCoeff ) return;
        if ( marker.empty() )
            M_fieldDiffusionCoeff->on(_range=elements(M_fieldDiffusionCoeff->mesh()),_expr=__expr );
        else
            M_fieldDiffusionCoeff->on(_range=markedelements(M_fieldDiffusionCoeff->mesh(),marker),_expr=__expr );
    }
    template < typename ExprT >
    void updateReactionCoeff( vf::Expr<ExprT> const& __expr, std::string const& marker = "" )
    {
        if ( !M_fieldReactionCoeff ) return;
        if ( marker.empty() )
            M_fieldReactionCoeff->on(_range=elements(M_fieldReactionCoeff->mesh()),_expr=__expr );
        else
            M_fieldReactionCoeff->on(_range=markedelements(M_fieldReactionCoeff->mesh(),marker),_expr=__expr );
    }

    //void updateFromModelMaterials( ModelMaterials const& mat )
    //{
        //if ( mat.empty() ) return;
        //for( auto const& m : mat )
        //{
            //auto const& mat = m.second;
            //auto const& matmarker = m.first;
            //this->setCstDiffusionCoeff( mat.rho(),matmarker );
            //this->setCstReactionCoeff( mat.rho(),matmarker );
        //}
    //}


private :
    space_ptrtype M_space;

    std::map<std::string,double> M_cstDiffusionCoeff;// D
    element_ptrtype M_fieldDiffusionCoeff;// D
    std::map<std::string,double> M_cstReactionCoeff;// R
    element_ptrtype M_fieldReactionCoeff;// R

};

} // namespace FeelModels
} // namespace Feel

#endif // FEELPP_FLUIDMECHANICS_DENSITYVISCOSITYMODEL_H
