// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import { SortableElement, SortableElementProps } from 'react-sortable-hoc'

// Feature-specific components
import {
  Tile,
  TileActionsContainer,
  TileAction,
  TileFavicon,
  TileMenu,
  TileMenuItem,
} from '../../components/default'

// Icons
import EditIcon  from '../../components/default/gridSites/assets/edit'
import EditMenuIcon  from '../../components/default/gridSites/assets/edit-menu'
import TrashIcon  from '../../components/default/gridSites/assets/trash'

// Types
import * as newTabActions from '../../actions/new_tab_actions'
import * as gridSitesActions from '../../actions/grid_sites_actions'

import { getLocale } from '../../../common/locale'

interface Props {
  actions: typeof newTabActions & typeof gridSitesActions
  siteData: NewTab.Site
  disabled: boolean
  isDragging: boolean
}

interface State {
  showMenu: boolean
}

function generateGridSiteFavicon (site: NewTab.Site): string {
  if (site.favicon === '') {
    return `chrome://favicon/size/64@1x/${site.url}`
  }
  return site.favicon
}

class TopSite extends React.PureComponent<Props, State> {
  tileMenuRef: React.RefObject<any>
  constructor (props: Props) {
    super(props)
    this.state = {
      showMenu: false
    }
    this.tileMenuRef = React.createRef()
  }

  componentDidMount () {
    document.addEventListener('mousedown', this.handleClickOutside)
    document.addEventListener('mouseMove', this.handleMoveOutside)
    document.addEventListener('keydown', this.onKeyPressSettings)
  }

  componentWillUnmount () {
    document.removeEventListener('mousedown', this.handleClickOutside)
    document.removeEventListener('mousemove', this.handleMoveOutside)
    document.removeEventListener('keydown', this.onKeyPressSettings)
  }

  onKeyPressSettings = (event: KeyboardEvent) => {
    if (event.key === 'Escape') {
      this.setState({showMenu: false})
    }
  }

  handleClickOutside = (event: Event) => {
    if (
      this.tileMenuRef &&
      this.tileMenuRef.current &&
      !this.tileMenuRef.current.contains(event.target)
    ) {
      this.setState({showMenu: false})
    }
  }

  handleMoveOutside = (event: Event) => {
    if (
      this.tileMenuRef &&
      this.tileMenuRef.current &&
      !this.tileMenuRef.current.contains(event.target)
    ) {
      this.setState({showMenu: false})
    }
  }

  onIgnoredTopSite (site: NewTab.Site) {
    this.props.actions.tileRemoved(site.url)
  }

  onEditTopSite (site: NewTab.Site) {
    this.props.actions.setShowEditTopSite(true, site)
  }

  render () {
    const { siteData, isDragging } = this.props
    return (
      <Tile
        title={siteData.title}
        tabIndex={0}
        isDragging={isDragging}
        isMenuShowing={this.state.showMenu}
      >
        {
          !siteData.defaultSRTopSite
          ? <TileActionsContainer>
              <TileAction onClick={() => this.setState({showMenu: true})}>
                <EditIcon/>
              </TileAction>
            </TileActionsContainer>
          : null
        }
        { this.state.showMenu &&
          <TileMenu innerRef={this.tileMenuRef}>
            <TileMenuItem onClick={this.onEditTopSite.bind(this, siteData)}>
              <EditMenuIcon />
              {getLocale('editSiteTileMenuItem')}
            </TileMenuItem>
            <TileMenuItem onClick={this.onIgnoredTopSite.bind(this, siteData)}>
              <TrashIcon />
              {getLocale('removeTileMenuItem')}
            </TileMenuItem>
          </TileMenu>
        }
        <a href={siteData.url}><TileFavicon src={generateGridSiteFavicon(siteData)} /></a>
      </Tile>
    )
  }
}

type TopSiteSortableElementProps = SortableElementProps & Props
export default SortableElement(
  (props: TopSiteSortableElementProps) => <TopSite {...props} />
)
